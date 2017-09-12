/**
 *******************************************************************************
 * @file        PPP.c
 * @version     0.0.2
 * @date        2017.09.12
 * @author      M. Strosche
 * @brief       Source file of the PPP-Protocol-Stack.
 *              This module implements the PPP-Protocol-Stack for the
 *              Data-Link-Layer of the OSI-Model. It receives data from a serial
 *              connection and parses it byte-by-byte. Valid payload-data will
 *              be transfered to a higher level by executing a
 *              callback-function. The frame-format of the PPP-packets is HDLC.
 *
 * @since       V0.0.2, 2017.09.12:
 *                      -# Modified doxygen-comments. (MS)
 *
 * @since       V0.0.1, 2017.09.11:
 *                      -# Initial version. (MS)
 *
 * @copyright   The MIT License (MIT)                                         @n
 *                                                                            @n
 *              Copyright (c) 2017 Michael Strosche                           @n
 *                                                                            @n
 *              Permission is hereby granted, free of charge, to any person
 *              obtaining a copy of this software and associated documentation
 *              files (the "Software"), to deal in the Software without
 *              restriction, including without limitation the rights to use,
 *              copy, modify, merge, publish, distribute, sublicense, and/or
 *              sell copies of the Software, and to permit persons to whom the
 *              Software is furnished to do so, subject to the following
 *              conditions:                                                   @n
 *                                                                            @n
 *              The above copyright notice and this permission notice shall be
 *              included in all´copies or substantial portions of the
 *              Software.                                                     @n
 *                                                                            @n
 *              THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *              EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *              OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *              NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *              HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *              WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *              FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *              OTHER DEALINGS IN THE SOFTWARE.
 *
 *******************************************************************************
 */

#include "PPP.h"
#include "PPP_cfg.h"

#include "..\\..\\system.h"
#include "..\\..\\utils\\crc.h"
#include "..\\..\\utils\\databuffer.h"
#include "..\\..\\utils\\serialConsole.h"

#if NET_PPP_MTU_MAX < (576)
        #error "NET_PPP_MTU_MAX must be greater or equal to 576"
#endif

#define NET_PPP_UARTPREFIX \
        CONCAT2(NET_PPP_UARTTYPE, NET_PPP_UARTNUMBER)
#define NET_PPP_UARTINCLUDE \
        CREATEINCLUDEFILEWPATH(NET_PPP_UARTPATH, NET_PPP_UARTPREFIX)
#define net_PPP_uart_init \
        CONCAT2(NET_PPP_UARTPREFIX, _init)
#define net_PPP_uart_setRxFinishedCallback \
        CONCAT2(NET_PPP_UARTPREFIX, _setRxFinishedCallback)
#define net_PPP_uart_setTxFinishedCallback \
        CONCAT2(NET_PPP_UARTPREFIX, _setTxFinishedCallback)
#define net_PPP_uart_txByte \
        CONCAT2(NET_PPP_UARTPREFIX, _txByte)

#include NET_PPP_UARTINCLUDE

#define NET_PPP_ADDRESS         (0xFF)
#define NET_PPP_CONTROL         (0x03)
#define NET_PPP_FLAG            (0x7E)
#define NET_PPP_ESCAPE          (0x7D)
#define NET_PPP_ESCAPE_TRANS    (0x20)

#define HASTOBEESCAPED(c) \
        ((c == NET_PPP_FLAG) || (c == NET_PPP_ESCAPE))

// type-definitions
typedef enum {
  PPPState_Dead,
  PPPState_Establish,
  PPPState_Authenticate,
  PPPState_Network,
  PPPState_Terminate,
} net_PPP_state_e;

typedef enum {
  PPPtxState_Idle,
  PPPtxState_SOF_Flag,
  PPPtxState_Address,
  PPPtxState_Control,
  PPPtxState_ProtocolH,
  PPPtxState_ProtocolL,
  PPPtxState_Data,
  PPPtxState_FcsH,
  PPPtxState_FcsL,
  PPPtxState_EOF_Flag
} net_PPP_txState_e;

typedef enum {
  PPPrxState_WaitingForSync,
  PPPrxState_SOF_Flag,
  PPPrxState_Address,
  PPPrxState_Control,
  PPPrxState_ProtocolH,
  PPPrxState_ProtocolL,
  PPPrxState_Data,
  PPPrxState_FcsH,
  PPPrxState_FcsL,
  PPPrxState_EOF_Flag
} net_PPP_rxState_e;

typedef union {
  uint32_t raw;
  uint8_t  b[sizeof(uint32_t)];
} net_PPP_lastReceivedBytes_t;

// private function prototypes
static void rxFinishedCallback(uint8_t b);
static void txFinishedCallback(void);
inline static void txByte(uint8_t b);
static void rxCallback_DUMMY(databuffer_basic_t *rxDataBuffer);

// private data
static net_PPP_state_e PPPstate;
static databuffer_basic_t *txDataBuffer;
static uint16_t txDataBufferReadIndex;
static net_PPP_txState_e txState;
static net_PPP_protocol_e txProtocol;
static crc16_t txFCScalc;
static crc16_t txFCSvalue;
static uint8_t txEscapeCharacter;
static uint8_t rxBuffer[NET_PPP_RX_PACKET_BUFFER_SIZE][NET_PPP_MTU_MAX + 2];
static databuffer_basic_t rxDataBuffer[NET_PPP_RX_PACKET_BUFFER_SIZE];
static uint16_t rxDataBufferWriteIndex;
static uint8_t numberOfRxPackets;
static uint8_t indexOfLastRxPacket;
static uint8_t indexOfFirstEmptyPacket;
static net_PPP_rxState_e rxState;
static crc16_t rxFCScalc;
static crc16_t rxFCSvalue;
static crc16_t rxFCS;
static uint8_t rxEscapeCharacter;
static net_PPP_protocol_e rxProtocol[NET_PPP_RX_PACKET_BUFFER_SIZE];
static net_PPP_lastReceivedBytes_t rxLastBytes;
static uint16_t mtuSize;
// RX-Callback-Functions
static void (*rxCallback_IP)(databuffer_basic_t *rxDataBuffer) =
        rxCallback_DUMMY;
static void (*rxCallback_LCP)(databuffer_basic_t *rxDataBuffer) =
        rxCallback_DUMMY;

// public functions
void net_PPP_init(void)
{
        // TODO: LPC wird auch benötigt!
        net_PPP_uart_init(NET_PPP_BAUDRATE);
        net_PPP_uart_setRxFinishedCallback(rxFinishedCallback);
        net_PPP_uart_setTxFinishedCallback(txFinishedCallback);

        PPPstate = PPPState_Dead;

        mtuSize = NET_PPP_MTU_MAX;

        txDataBuffer = NULL;
        txState = PPPtxState_Idle;

        for (uint8_t i=0; i<NET_PPP_RX_PACKET_BUFFER_SIZE; i++)
                databuffer_create(&(rxDataBuffer[i]),
                                  rxBuffer[i],
                                  NET_PPP_MTU_MAX);

        rxState = PPPrxState_WaitingForSync;
        rxLastBytes.raw = 0;

        numberOfRxPackets = 0;
        indexOfLastRxPacket = 0;
        indexOfFirstEmptyPacket = 0;
}

void net_PPP_loop(void)
{
        while (numberOfRxPackets > 0) {
                switch (rxProtocol[indexOfLastRxPacket]) {
                        case NETPPP_IP:
                                rxCallback_IP(&(rxDataBuffer[indexOfLastRxPacket]));
                                break;
                        case NETPPP_LCP:
                                rxCallback_LCP(&(rxDataBuffer[indexOfLastRxPacket]));
                                break;
                        default:
                                rxCallback_DUMMY(&(rxDataBuffer[indexOfLastRxPacket]));
                                break;
                }

                indexOfLastRxPacket = (indexOfLastRxPacket + 1)
                                      % NET_PPP_RX_PACKET_BUFFER_SIZE;
                numberOfRxPackets--;
        }
}

void net_PPP_txDataBuffer(net_PPP_protocol_e protocol,
                          databuffer_basic_t *dataBufferChain)
{
        if ((txDataBuffer == NULL) && (dataBufferChain->tot_length > 0)) {
                txDataBuffer = dataBufferChain;
                txDataBufferReadIndex = 0;
                txProtocol = protocol;
                txEscapeCharacter = 0;

                // Transmit SOF-Flag.
                net_PPP_uart_txByte(NET_PPP_FLAG);
                txState = PPPtxState_SOF_Flag;
        }
}

bool net_PPP_txIsBusy(void)
{
        return txDataBuffer != NULL;
}

void net_PPP_setLCPRxCallback(void (*rxCallback)(databuffer_basic_t *rxDataBuffer))
{
        if (rxCallback != NULL)
                rxCallback_LCP = rxCallback;
}

void net_PPP_setIPRxCallback(void (*rxCallback)(databuffer_basic_t *rxDataBuffer))
{
        if (rxCallback != NULL)
                rxCallback_IP = rxCallback;
}


// private functions
static void rxFinishedCallback(uint8_t b)
{
        bool hasFlag = false;

        if (b == NET_PPP_ESCAPE) {
                rxEscapeCharacter = NET_PPP_ESCAPE;
        } else {
                if (rxEscapeCharacter != 0)
                        b = b ^ NET_PPP_ESCAPE_TRANS;
                else
                        hasFlag = (b == NET_PPP_FLAG);

#ifdef NET_PPP_RELAY_INSTREAM
                serialConsole_txByte(b);
#endif /* NET_PPP_RELAY_INSTREAM */

                rxEscapeCharacter = 0;

                switch (rxState) {
                        case PPPrxState_WaitingForSync:
                                if (hasFlag) {
                                        // Received valid Flag.
                                        rxState = PPPrxState_SOF_Flag;
                                }
                                break;
        
                        case PPPrxState_SOF_Flag:
                                if (hasFlag) {
                                        // Received valid Flag.
                                        rxState = PPPrxState_SOF_Flag;
                                } else if (b == NET_PPP_ADDRESS) {
                                        // Received Address-Byte.
                                        rxState = PPPrxState_Address;
                                } else {
                                        // Out of sync...
                                        rxState = PPPrxState_WaitingForSync;
                                }
                                break;
        
                        case PPPrxState_Address:
                                if (hasFlag) {
                                        // Received valid Flag.
                                        rxState = PPPrxState_SOF_Flag;
                                } else if (b == NET_PPP_CONTROL) {
                                        // Received Control-Byte.
                                        rxState = PPPrxState_Control;
                                } else {
                                        // Out of sync...
                                        rxState = PPPrxState_WaitingForSync;
                                }
                                break;
        
                        case PPPrxState_Control:
                                if (hasFlag) {
                                        // Received valid Flag.
                                        rxState = PPPrxState_SOF_Flag;
                                } else {
                                // Received first Protocol-Byte.
                                        rxProtocol[indexOfFirstEmptyPacket] =
                                                ((net_PPP_protocol_e)b) << 8;
                                        rxState = PPPrxState_ProtocolH;
                                }
                                break;
        
                        case PPPrxState_ProtocolH:
                                if (hasFlag) {
                                        // Received valid Flag.
                                        rxState = PPPrxState_SOF_Flag;
                                } else {
                                        // Received second Protocol-Byte.
                                        rxProtocol[indexOfFirstEmptyPacket] |=
                                                (net_PPP_protocol_e)b;
                                        rxState = PPPrxState_ProtocolL;
                                }
                                break;
        
                        case PPPrxState_ProtocolL:
                                if (hasFlag) {
                                        // Received valid Flag.
                                        rxState = PPPrxState_SOF_Flag;
                                } else {
                                        // Received first Data-Byte.
                                        rxDataBufferWriteIndex = 0;
                                        rxDataBuffer[indexOfFirstEmptyPacket].data[rxDataBufferWriteIndex++] =
                                                b;
                                        rxState = PPPrxState_Data;
                                }
                                break;
        
                        case PPPrxState_Data:
                                if (hasFlag) {
                                        // Received valid EOF-Flag.

                                        // Calculate crc over received data
                                        crc16_fcs_setSeed(&rxFCScalc);
                                        crc16_fcs_byte(&rxFCScalc,
                                                       NET_PPP_ADDRESS);
                                        crc16_fcs_byte(&rxFCScalc,
                                                       NET_PPP_CONTROL);
                                        crc16_fcs_byte(&rxFCScalc,
                                                       (rxProtocol[indexOfFirstEmptyPacket] >> 8) & 0x00FF);
                                        crc16_fcs_byte(&rxFCScalc,
                                                       (rxProtocol[indexOfFirstEmptyPacket] >> 0) & 0x00FF);
                                        crc16_fcs_data(&rxFCScalc,
                                                       rxDataBuffer[indexOfFirstEmptyPacket].data,
                                                       rxDataBufferWriteIndex - 2);
                                        rxFCSvalue = rxFCScalc ^ 0xFFFF;
                                        rxFCS = ((crc16_t)rxDataBuffer[indexOfFirstEmptyPacket].data[rxDataBufferWriteIndex - 1] << 8) |
                                                (crc16_t)rxDataBuffer[indexOfFirstEmptyPacket].data[rxDataBufferWriteIndex - 2];
                                        //rxFCS = rxFCSvalue;

                                        if (rxFCS == rxFCSvalue) {
                                                // Received valid ppp-packet.
                                                rxDataBuffer[indexOfFirstEmptyPacket].length =
                                                        rxDataBufferWriteIndex - 2;
                                                rxDataBuffer[indexOfFirstEmptyPacket].tot_length =
                                                        rxDataBufferWriteIndex - 2;

                                                indexOfFirstEmptyPacket =
                                                        (indexOfFirstEmptyPacket + 1)
                                                        % NET_PPP_RX_PACKET_BUFFER_SIZE;
                                                numberOfRxPackets++;
                                        } else {
                                                // No valid ppp-packet received.
                                                serialConsole_txString("\n\n");
                                                serialConsole_txByte((rxProtocol[indexOfFirstEmptyPacket] >> 8) & 0x00FF);
                                                serialConsole_txByte((rxProtocol[indexOfFirstEmptyPacket] >> 0) & 0x00FF);
                                                serialConsole_txByte((rxFCS >> 8) & 0x00FF);
                                                serialConsole_txByte((rxFCS >> 0) & 0x00FF);
                                                serialConsole_txByte((rxFCSvalue >> 8) & 0x00FF);
                                                serialConsole_txByte((rxFCSvalue >> 0) & 0x00FF);
                                                serialConsole_txString("\n\n");
                                        }
                                        
                                        rxState = PPPrxState_SOF_Flag;
                                } else if (rxDataBufferWriteIndex < mtuSize) {
                                        // Received n-th Data-Byte.
                                        rxDataBuffer[indexOfFirstEmptyPacket].data[rxDataBufferWriteIndex++] = b;
                                } else {
                                        // Reached mtu-limit.
                                        
                                        // Calculate crc over received data
                                        crc16_fcs_setSeed(&rxFCScalc);
                                        crc16_fcs_byte(&rxFCScalc,
                                                       NET_PPP_ADDRESS);
                                        crc16_fcs_byte(&rxFCScalc,
                                                       NET_PPP_CONTROL);
                                        crc16_fcs_byte(&rxFCScalc,
                                                       (rxProtocol[indexOfFirstEmptyPacket] >> 8) & 0x00FF);
                                        crc16_fcs_byte(&rxFCScalc,
                                                       (rxProtocol[indexOfFirstEmptyPacket] >> 0) & 0x00FF);
                                        crc16_fcs_data(&rxFCScalc,
                                                       rxDataBuffer[indexOfFirstEmptyPacket].data,
                                                       rxDataBufferWriteIndex);
                                        rxFCSvalue = rxFCScalc ^ 0xFFFF;
                                        
                                        // Received first FCS-Byte.
                                        rxFCS = (crc16_t)b;
                                        rxState = PPPrxState_FcsL;
                                }
                                break;
        
                        case PPPrxState_FcsL:
                                if (hasFlag) {
                                        // Received valid Flag.
                                        rxState = PPPrxState_SOF_Flag;
                                } else {
                                        // Received second FCS-Byte.
                                        rxFCS |= ((crc16_t)b) << 8;

                                        if (rxFCS == rxFCSvalue)
                                                rxState = PPPrxState_FcsH;
                                        else
                                                // Out of sync...
                                                rxState = PPPrxState_WaitingForSync;
                                }
                                break;
        
                        case PPPrxState_FcsH:
                                if (hasFlag) {
                                        // Received valid Flag.
                                        rxState = PPPrxState_SOF_Flag;

                                        rxDataBuffer[indexOfFirstEmptyPacket].length =
                                                rxDataBufferWriteIndex;
                                        rxDataBuffer[indexOfFirstEmptyPacket].tot_length =
                                                rxDataBufferWriteIndex;

                                        indexOfFirstEmptyPacket =
                                                (indexOfFirstEmptyPacket + 1)
                                                % NET_PPP_RX_PACKET_BUFFER_SIZE;
                                        numberOfRxPackets++;
                                } else {
                                        // Out of sync...
                                        rxState = PPPrxState_WaitingForSync;
                                }
                                break;
                                
                        case PPPrxState_EOF_Flag:
                                break;
                }
        }
}

static void txFinishedCallback(void)
{
        if (txEscapeCharacter == 0) {
                switch (txState) {
                        case PPPtxState_SOF_Flag:
                                // Start CRC-Calculation.
                                crc16_fcs_setSeed(&txFCScalc);

                                // Transmit Address-Byte.
                                txByte(NET_PPP_ADDRESS);
                                txState = PPPtxState_Address;
          
                                break;
  
                        case PPPtxState_Address:
                                // Transmit Control-Byte.
                                txByte(NET_PPP_CONTROL);
                                txState = PPPtxState_Control;

                                break;
          
                        case PPPtxState_Control:
                                // Transmit first Protocol-Byte.
                                txByte(((uint16_t)txProtocol >> 8) & 0x00FF);
                                txState = PPPtxState_ProtocolH;

                                break;
          
                        case PPPtxState_ProtocolH:
                                // Transmit second Protocol-Byte.
                                txByte(((uint16_t)txProtocol >> 0) & 0x00FF);
                                txState = PPPtxState_ProtocolL;

                                break;
          
                        case PPPtxState_ProtocolL:
                                // Transmit first Data-Byte.
                                txByte(txDataBuffer->data[txDataBufferReadIndex++]);

                                if (txDataBufferReadIndex >= txDataBuffer->length) {
                                        txDataBuffer = txDataBuffer->next;
                                        txDataBufferReadIndex = 0;
                                }

                                txState = PPPtxState_Data;

                                break;
          
                        case PPPtxState_Data:
                                if (txDataBuffer != NULL) {
                                        // Transmit n-th Data-Byte.
                                        txByte(txDataBuffer->data[txDataBufferReadIndex++]);

                                        if (txDataBufferReadIndex >= txDataBuffer->length) {
                                                txDataBuffer = txDataBuffer->next;
                                                txDataBufferReadIndex = 0;
                                        }
                                } else {
                                        txFCSvalue = txFCScalc ^ 0xFFFF;

                                        // Transmit first FCS-Byte.
                                        txByte(((uint16_t)txFCSvalue >> 0) & 0x00FF);
                                        txState = PPPtxState_FcsH;
                                }

                                break;
          
                        case PPPtxState_FcsH:
                                // Transmit second FCS-Byte.
                                txByte(((uint16_t)txFCSvalue >> 8) & 0x00FF);
                                txState = PPPtxState_FcsL;

                                break;
          
                        case PPPtxState_FcsL:
                                // Transmit EOF Flag.
                                net_PPP_uart_txByte(NET_PPP_FLAG);
                                txState = PPPtxState_EOF_Flag;

                                break;
          
                        case PPPtxState_EOF_Flag:
                                // End of Transmission.
                        default:
                                txDataBuffer = NULL;
                                txState = PPPtxState_Idle;
                  
                                break;
                }
        } else {
                net_PPP_uart_txByte(txEscapeCharacter ^ NET_PPP_ESCAPE_TRANS);
                txEscapeCharacter = 0;
        }
}

inline static void txByte(uint8_t b)
{
        crc16_fcs_byte(&txFCScalc, b);

        if ((b == NET_PPP_FLAG) || (b == NET_PPP_ESCAPE)) {
                txEscapeCharacter = b;
                b = NET_PPP_ESCAPE;
        }

        net_PPP_uart_txByte(b);
}

static void rxCallback_DUMMY(databuffer_basic_t *rxDataBuffer)
{
        serialConsole_txString("\nrecv_?:");
        serialConsole_txDatabuffer(rxDataBuffer);
        serialConsole_txString("\n");
}


// interrupt service routines
