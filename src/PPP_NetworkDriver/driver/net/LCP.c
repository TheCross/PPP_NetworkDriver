// info: http://www.nwlab.net/art/pppoe/

/**
 *******************************************************************************
 * @file        LCP.c
 * @version     0.0.3
 * @date        2017.09.25
 * @author      M. Strosche
 * @brief       Source file of the LCP-Protocol-Stack.
 *
 * @since       V0.0.3, 2017.09.25:
 *                      -# Added handling of incomming LCP-Options for
 *                         configuration. (MS)
 *                      -# No typedefs for struct and enum. (MS)
 *                      -# Added net_LCP_startConfigurationOfHost. (MS)
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

#include "LCP.h"
#include "LCP_cfg.h"

#include "..\\..\\system.h"
#include "..\\..\\utils\\databuffer.h"
#include "..\\..\\utils\\serialConsole.h"

#undef NET_LPC_DATALINK_FILENAME
#define NET_LPC_DATALINK_FILENAME \
        CONCAT1(NET_LCP_DATALINK)
#define NET_LCP_DATALINKINCLUDE \
        CREATEINCLUDEFILEWPATH(NET_LCP_DATALINKPATH, NET_LPC_DATALINK_FILENAME)
#include NET_LCP_DATALINKINCLUDE

#define NET_LCP_DATALINK_FUNPREFIX \
        CONCAT2(net_, NET_LCP_DATALINK)
#define net_LCP_datalink_setIPRxCallback \
        CONCAT2(NET_LCP_DATALINK_FUNPREFIX, _setLCPRxCallback)
#define net_LCP_datalink_txDataBuffer \
        CONCAT2(NET_LCP_DATALINK_FUNPREFIX, _txDataBuffer)
#define net_LCP_datalink_setMtuSize \
        CONCAT2(NET_LCP_DATALINK_FUNPREFIX, _setMtuSize)
        
#define NET_LCP_DATALINK_CONSTPREFIX \
        CONCAT2(NET_, NET_LCP_DATALINK)
#define net_LCP_datalink_maxMTU() \
        CONCAT2(NET_LCP_DATALINK_CONSTPREFIX, _MTU_MAX)
        

#define NET_LCP_HEADER_LENGTH               (4)

// type-definitions
enum net_LCP_code_e {
        LCP_ConfigureRequest = 1,
        LCP_ConfigureAck = 2,
        LCP_ConfigureNak = 3,
        LCP_ConfigureReject = 4,
        LCP_TerminateRequest = 5,
        LCP_TerminateAck = 6,
        LCP_CodeReject = 7,
        LCP_ProtocolReject = 8,
        LCP_EchoRequest = 9,
        LCP_EchoReply = 10,
        LCP_DiscardRequest = 11
};

struct net_LCP_Option_t {
        uint8_t  type;
        uint8_t  length;
        uint8_t  data[];
};

enum net_LCP_option_e {
        LCP_OPTION_MRU = 1,
        LCP_OPTION_ACCM = 2,
        LCP_OPTION_AuthProtocol = 3,
        LCP_OPTION_MagicNumber = 5,
        LCP_OPTION_ProtocolCompression = 7,
        LCP_OPTION_AddressAndControlCompression = 8,
        LCP_OPTION_Callback = 13
};
#define LCP_OPTION_LENGTH_MRU                           (4)
#define LCP_OPTION_LENGTH_ACCM                          (6)
#define LCP_OPTION_LENGTH_AuthProtocol                  ()
#define LCP_OPTION_LENGTH_MagicNumber                   (6)
#define LCP_OPTION_LENGTH_ProtocolCompression           (2)
#define LCP_OPTION_LENGTH_AddressAndControlCompression  (2)
#define LCP_OPTION_LENGTH_Callback                      ()

// private function prototypes
static void rxCallback(struct databuffer_basic_t *rxDataBuffer);
static void sendMessage(enum net_LCP_code_e code,
                        uint8_t identifier,
                        struct databuffer_basic_t *data);
static void handleConfigureRequest(uint8_t identifier,
                                   struct databuffer_basic_t *rxOptions);
static void handleConfigureAck(uint8_t identifier,
                               struct databuffer_basic_t *rxOptions);
static void handleConfigureNak(uint8_t identifier,
                               struct databuffer_basic_t *rxOptions);
static void handleConfigureReject(uint8_t identifier,
                                  struct databuffer_basic_t *rxOptions);
static bool checkConfigureRequest(enum net_LCP_code_e mode,
                                  uint8_t identifier,
                                  struct databuffer_basic_t *rxOptions);
#define net_LCP_getMagicNumber()        \
        (~peerMagicNumber)

// private data
static uint8_t txDataHeader[NET_LCP_HEADER_LENGTH];
static struct databuffer_basic_t txDataBufferHeader;
static uint8_t txData[64];
static struct databuffer_basic_t txDataBuffer;
static struct databuffer_basic_t txOptionResponse;
static uint32_t peerMagicNumber;
static uint8_t state;
static uint8_t rxIdentifier;

// public functions
void net_LCP_init(void)
{
        net_LCP_datalink_setIPRxCallback(rxCallback);

        databuffer_create(&txDataBuffer,
                          txData,
                          64);
        peerMagicNumber = 0xCAFEBABE;
        
        rxIdentifier = 0;
        state = 0;
}

uint8_t net_LCP_getState(void)
{
        return state;
}

void net_LCP_startConfigurationOfHost(void)
{
        struct net_LCP_Option_t *option = (struct net_LCP_Option_t *)txDataBuffer.data;
        txDataBuffer.length = 0;
        
        // create the configuration-data
        //  magic-number
        option->type    = LCP_OPTION_MagicNumber;
        option->length  = LCP_OPTION_LENGTH_MagicNumber;
        option->data[0] = (uint8_t)((net_LCP_getMagicNumber() >> 24)
                                    & 0x000000FF);
        option->data[1] = (uint8_t)((net_LCP_getMagicNumber() >> 16)
                                    & 0x000000FF);
        option->data[2] = (uint8_t)((net_LCP_getMagicNumber() >>  8)
                                    & 0x000000FF);
        option->data[3] = (uint8_t)((net_LCP_getMagicNumber() >>  0)
                                    & 0x000000FF);
        txDataBuffer.length += option->length;
        option = (struct net_LCP_Option_t *)(txDataBuffer.data
                                             + txDataBuffer.length);
        
        //  ACCM
        option->type    = LCP_OPTION_ACCM;
        option->length  = LCP_OPTION_LENGTH_ACCM;
        option->data[0] = 0x00;
        option->data[1] = 0x00;
        option->data[2] = 0x00;
        option->data[3] = 0x00;
        txDataBuffer.length += option->length;
        option = (struct net_LCP_Option_t *)(txDataBuffer.data
                                             + txDataBuffer.length);
        
        //  MRU
        option->type    = LCP_OPTION_MRU;
        option->length  = LCP_OPTION_LENGTH_MRU;
        option->data[0] = (uint8_t)((net_LCP_datalink_maxMTU() >> 8) & 0x00FF);
        option->data[1] = (uint8_t)((net_LCP_datalink_maxMTU() >> 0) & 0x00FF);
        txDataBuffer.length += option->length;
        option = (struct net_LCP_Option_t *)(txDataBuffer.data
                                             + txDataBuffer.length);
        
        // send the configuration-data
        txDataBuffer.tot_length = txDataBuffer.length;
        sendMessage(LCP_ConfigureRequest, rxIdentifier, &txDataBuffer);
}


// private functions
static void rxCallback(struct databuffer_basic_t *rxDataBuffer)
{
        uint8_t identifier = rxDataBuffer->data[1];
        uint16_t length = (((uint16_t)rxDataBuffer->data[2] << 8) |
                           ((uint16_t)rxDataBuffer->data[3] << 0))
                          - NET_LCP_HEADER_LENGTH;

        databuffer_copy_partial(&txDataBuffer,
                                0,
                                rxDataBuffer,
                                NET_LCP_HEADER_LENGTH,
                                length);
        txDataBuffer.tot_length = length;
        txDataBuffer.length = length;

        switch ((enum net_LCP_code_e)rxDataBuffer->data[0]) {
        case LCP_ConfigureRequest:
                serialConsole_txString("\nLCP_ConfigureRequest:");
                serialConsole_txDatabuffer(&txDataBuffer);
                  
                handleConfigureRequest(identifier, &txDataBuffer);
                break;

        case LCP_ConfigureAck:
                serialConsole_txString("\nLCP_ConfigureAck:");
                serialConsole_txDatabuffer(&txDataBuffer);
                  
                handleConfigureAck(identifier, &txDataBuffer);
                break;

        case LCP_ConfigureNak:
                serialConsole_txString("\nLCP_ConfigureNak:");
                serialConsole_txDatabuffer(&txDataBuffer);
                  
                handleConfigureNak(identifier, &txDataBuffer);
                break;

        case LCP_ConfigureReject:
                serialConsole_txString("\nLCP_ConfigureReject:");
                serialConsole_txDatabuffer(&txDataBuffer);
                  
                handleConfigureReject(identifier, &txDataBuffer);
                break;

        case LCP_TerminateRequest:
                serialConsole_txString("\nTerminateRequest:");
                serialConsole_txDatabuffer(&txDataBuffer);
                break;

        case LCP_TerminateAck:
                serialConsole_txString("\nLCP_TerminateAck:");
                serialConsole_txDatabuffer(&txDataBuffer);
                break;

        case LCP_CodeReject:
                serialConsole_txString("\nLCP_CodeReject:");
                serialConsole_txDatabuffer(&txDataBuffer);
                break;

        case LCP_ProtocolReject:
                serialConsole_txString("\nLCP_ProtocolReject:");
                serialConsole_txDatabuffer(&txDataBuffer);
                break;

        case LCP_EchoRequest:
                serialConsole_txString("\nLCP_EchoRequest:");
                serialConsole_txDatabuffer(&txDataBuffer);
                break;

        case LCP_EchoReply:
                serialConsole_txString("\nLCP_EchoReply:");
                serialConsole_txDatabuffer(&txDataBuffer);
                break;

        case LCP_DiscardRequest:
                serialConsole_txString("\nLCP_DiscardRequest:");
                serialConsole_txDatabuffer(&txDataBuffer);
                break;

        default:
                serialConsole_txString("\nLCP_unknown:");
                serialConsole_txDatabuffer(&txDataBuffer);
                break;
        }

        serialConsole_txString("\n");
}

static void sendMessage(enum net_LCP_code_e code,
                           uint8_t identifier,
                           struct databuffer_basic_t *data)
{
        uint16_t length = data->tot_length + NET_LCP_HEADER_LENGTH;
        
        databuffer_create(&txDataBufferHeader,
                          txDataHeader,
                          NET_LCP_HEADER_LENGTH);
        txDataBufferHeader.data[0] = code;
        txDataBufferHeader.data[1] = identifier;
        txDataBufferHeader.data[2] = (length >> 8) & 0x00FF;
        txDataBufferHeader.data[3] = (length >> 0) & 0x00FF;

        databuffer_insertAtEnd(&txDataBufferHeader, data);

        net_LCP_datalink_txDataBuffer(NETPPP_LCP, &txDataBufferHeader);
}

static void handleConfigureRequest(uint8_t identifier,
                                   struct databuffer_basic_t *rxOptions)
{
        // (1) check for options to reject
        if (checkConfigureRequest(LCP_ConfigureReject,
                                  identifier,
                                  rxOptions)) {
                // (2) if !(1) check for options to nak 
                if (checkConfigureRequest(LCP_ConfigureNak,
                                          identifier,
                                          rxOptions)) {
                        // (3) if !(1) && !(2) set values
                        checkConfigureRequest(LCP_ConfigureAck,
                                              identifier,
                                              rxOptions);
                        // At this point the configuration of the client
                        // (this machine) has been finished!
                        state |= NET_LCP_STATE__CLIENT_CONFIGURED;
                }
        }
}

static void handleConfigureAck(uint8_t identifier,
                               struct databuffer_basic_t *rxOptions)
{
        // TODO: handleConfigureAck needs implementation!
        state |= NET_LCP_STATE__HOST_CONFIGURED;
}

static void handleConfigureNak(uint8_t identifier,
                               struct databuffer_basic_t *rxOptions)
{
        // TODO: handleConfigureNak needs implementation!
}

static void handleConfigureReject(uint8_t identifier,
                                  struct databuffer_basic_t *rxOptions)
{
        // TODO: handleConfigureReject needs implementation!
}

// see https://technet.microsoft.com/en-us/library/cc957992.aspx
static bool checkConfigureRequest(enum net_LCP_code_e mode,
                                  uint8_t identifier,
                                  struct databuffer_basic_t *rxOptions)
{
        uint16_t tempShort;
        uint16_t optionReadPosition = 0;
        uint16_t optionWritePosition = 0;
        struct net_LCP_Option_t *option = rxOptions->data;
        
        while (optionReadPosition < rxOptions->length) {
                option = (struct net_LCP_Option_t *)(rxOptions->data +
                                                     optionReadPosition);
                switch (option->type) {
                case LCP_OPTION_MRU:
                        tempShort = (((uint16_t)option->data[0] << 8) |
                                     ((uint16_t)option->data[1] << 0));
                        if (mode == LCP_ConfigureAck) {
                                // set the new value
                                net_LCP_datalink_setMtuSize(tempShort);
                        } else if ((mode == LCP_ConfigureNak) &&
                                   (tempShort > net_LCP_datalink_maxMTU())) {
                                // set possible value
                                option->data[0] =
                                        (net_LCP_datalink_maxMTU() >> 8)
                                        & 0x00FF;
                                option->data[1] =
                                        (net_LCP_datalink_maxMTU() >> 0)
                                        & 0x00FF;
                                        
                                // copy option to the front of the buffer
                                memcpy(&rxOptions->data[optionWritePosition],
                                       &rxOptions->data[optionReadPosition],
                                       option->length);
                                optionWritePosition += option->length;
                        }
                        break;
                        
                case LCP_OPTION_ACCM:
                        // TODO: LCP_OPTION_ACCM
                        break;
                        
                case LCP_OPTION_AuthProtocol:
                        // TODO: LCP_OPTION_AuthProtocol
                        break;
                        
                case LCP_OPTION_MagicNumber:
                        if (mode == LCP_ConfigureAck) {
                                peerMagicNumber = (((uint32_t)option->data[0] << 24) |
                                                   ((uint32_t)option->data[1] << 16) |
                                                   ((uint32_t)option->data[2] <<  8) |
                                                   ((uint32_t)option->data[3] <<  0));
                        }
                        break;
                        
                case LCP_OPTION_AddressAndControlCompression:
                        // TODO: Reject Compression!
                        //  we do not support this at the moment
                        // but it may be implemented in the future...
                        
                case LCP_OPTION_ProtocolCompression:
                        // Reject Compression!
                        //  we do not support this at the moment
                        
                case LCP_OPTION_Callback:
                        // TODO: Reject Callbacks!
                        //  we do not support this at the moment
                        
                default:
                        if (mode == LCP_ConfigureReject) {
                                // copy option to the front of the buffer
                                memcpy(&rxOptions->data[optionWritePosition],
                                       &rxOptions->data[optionReadPosition],
                                       option->length);
                                optionWritePosition += option->length;
                        }
                        break;
                }
                
                optionReadPosition += option->length;
        }
        
        // Send answer if:
        //  (1) Mode is LCP_ConfigureReject and we found some options to reject
        //      (optionWritePosition != 0)
        //  (2) Mode is LCP_ConfigureNak and we found some values to nak
        //      (optionWritePosition != 0)
        //  (3) Mode is LCP_ConfigureAck.
        if (mode != LCP_ConfigureAck) {
                if (optionWritePosition != 0) {
                        rxOptions->length = optionWritePosition;
                        rxOptions->tot_length = optionWritePosition;
                        
                        sendMessage(mode, identifier, rxOptions);
                }
        } else {
                sendMessage(mode, identifier, rxOptions);
        }
        
        // if the write position changed we had to correct for errors, etc...
        return optionWritePosition == 0;
}


// interrupt service routines
