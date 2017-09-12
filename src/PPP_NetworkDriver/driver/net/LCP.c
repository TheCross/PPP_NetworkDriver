/**
 *******************************************************************************
 * @file        LCP.c
 * @version     0.0.2
 * @date        2017.09.12
 * @author      M. Strosche
 * @brief       Source file of the LCP-Protocol-Stack.
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

#define NET_LCP_HEADER_LENGTH               (4)

// type-definitions
typedef enum {
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
} net_PPP_LCP_code_e;

// private function prototypes
static void rxCallback(databuffer_basic_t *rxDataBuffer);
static void sendResponse(net_PPP_LCP_code_e code,
                         uint8_t identifier,
                         databuffer_basic_t *data);

// private data
static uint8_t txDataHeader[NET_LCP_HEADER_LENGTH];
static databuffer_basic_t txDataBufferHeader;
static uint8_t txData[64];
static databuffer_basic_t txDataBuffer;

// public functions
void net_LCP_init(void)
{
        net_LCP_datalink_setIPRxCallback(rxCallback);

        databuffer_create(&txDataBufferHeader,
                          txDataHeader,
                          NET_LCP_HEADER_LENGTH);
        databuffer_create(&txDataBuffer,
                          txData,
                          64);
}


// private functions
static void rxCallback(databuffer_basic_t *rxDataBuffer)
{
        net_PPP_LCP_code_e code = rxDataBuffer->data[0];
        uint8_t identifier = rxDataBuffer->data[1];
        uint16_t length = (((uint16_t)rxDataBuffer->data[2] << 8) |
                           ((uint16_t)rxDataBuffer->data[3]))
                          - NET_LCP_HEADER_LENGTH;

        databuffer_copy_partial(&txDataBuffer,
                                0,
                                rxDataBuffer,
                                NET_LCP_HEADER_LENGTH,
                                length);
        txDataBuffer.tot_length = length;
        txDataBuffer.length = length;

        switch (code) {
                case LCP_ConfigureRequest:
                        serialConsole_txString("\nLCP_ConfigureRequest:");
                        serialConsole_txDatabuffer(&txDataBuffer);
                          
                        sendResponse(LCP_ConfigureAck,
                                     identifier,
                                     &txDataBuffer);
                        break;

                case LCP_ConfigureAck:
                        serialConsole_txString("\nLCP_ConfigureAck:");
                        serialConsole_txDatabuffer(&txDataBuffer);
                        break;

                case LCP_ConfigureNak:
                        serialConsole_txString("\nLCP_ConfigureNak:");
                        serialConsole_txDatabuffer(&txDataBuffer);
                        break;

                case LCP_ConfigureReject:
                        serialConsole_txString("\nLCP_ConfigureReject:");
                        serialConsole_txDatabuffer(&txDataBuffer);
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

static void sendResponse(net_PPP_LCP_code_e code,
                         uint8_t identifier,
                         databuffer_basic_t *data)
{
        uint16_t length = data->tot_length + NET_LCP_HEADER_LENGTH;

        txDataBufferHeader.data[0] = code;
        txDataBufferHeader.data[1] = identifier;
        txDataBufferHeader.data[2] = (length >> 8) & 0x00FF;
        txDataBufferHeader.data[3] = (length >> 0) & 0x00FF;

        databuffer_insertAtEnd(&txDataBufferHeader, data);

        //serialConsole_txString("\response:");
        //serialConsole_txDatabuffer(&txDataBufferHeader);
        net_PPP_txDataBuffer(NETPPP_LCP, &txDataBufferHeader);
}


// interrupt service routines
