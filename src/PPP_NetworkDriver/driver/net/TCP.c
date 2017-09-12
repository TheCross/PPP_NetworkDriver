/**
 *******************************************************************************
 * @file        TCP.c
 * @version     0.0.2
 * @date        2017.09.12
 * @author      M. Strosche
 * @brief       Source file of the TCP-Protocol-Stack.
 *              This module implements the TCP-Protocol-Stack for the
 *              Transport-Layer of the OSI-Model.
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

#include "TCP.h"
#include "TCP_cfg.h"

#include "..\\..\\system.h"
#include "..\\..\\utils\\databuffer.h"
#include "..\\..\\utils\\serialConsole.h"

#undef NET_TPC_DATALINK_FILENAME
#define NET_TPC_DATALINK_FILENAME \
        CONCAT1(NET_TCP_INTERNET)
#define NET_TCP_INTERNETINCLUDE \
        CREATEINCLUDEFILEWPATH(NET_TCP_INTERNETPATH, NET_TPC_DATALINK_FILENAME)
#include NET_TCP_INTERNETINCLUDE

#undef NET_TPC_DATALINK_FILENAME
#define NET_TPC_DATALINK_FILENAME \
        CONCAT2(NET_TCP_INTERNET, _common)
#define NET_TCP_INTERNETINCLUDECOMMON \
        CREATEINCLUDEFILEWPATH(NET_TCP_INTERNETPATH, NET_TPC_DATALINK_FILENAME)
#include NET_TCP_INTERNETINCLUDECOMMON

#define NET_TCP_INTERNET_FUNPREFIX \
        CONCAT2(net_, NET_TCP_INTERNET)
#define net_TCP_internet_setTCPRxCallback \
        CONCAT2(NET_TCP_INTERNET_FUNPREFIX, _setTCPRxCallback)

// type-definitions
typedef struct {
        uint32_t raw[5];
} tcp_header_t;


// private function prototypes
static void rxCallback(databuffer_basic_t *rxDataBuffer, ipv4_t sourceIP);

inline uint16_t tcp_getSourcePort(tcp_header_t *tcp_header)
{
        return (uint16_t)((tcp_header->raw[0] >> 0) & 0x0000FFFF);
}

inline uint16_t tcp_getDestinationPort(tcp_header_t *tcp_header)
{
        return (uint16_t)((tcp_header->raw[0] >> 16) & 0x0000FFFF);
}

inline uint32_t tcp_getSequenceNumber(tcp_header_t *tcp_header)
{
        return tcp_header->raw[1];
}

inline uint32_t tcp_getAcknowledgeNumber(tcp_header_t *tcp_header)
{
        return tcp_header->raw[2];
}

inline uint8_t tcp_getDataOffset(tcp_header_t *tcp_header)
{
        return (uint8_t)((tcp_header->raw[3] >> 0) & 0x0000000F);
}

inline uint8_t tcp_getControlFlags(tcp_header_t *tcp_header)
{
        return (uint8_t)((tcp_header->raw[3] >> 8) & 0x000000FF);
}

inline uint16_t tcp_getWindow(tcp_header_t *tcp_header)
{
        return (uint16_t)((tcp_header->raw[3] >> 16) & 0x0000FFFF);
}

inline uint16_t tcp_getChecksum(tcp_header_t *tcp_header)
{
        return (uint16_t)((tcp_header->raw[4] >> 0) & 0x0000FFFF);
}

inline uint16_t tcp_getUrgentPointer(tcp_header_t *tcp_header)
{
        return (uint16_t)((tcp_header->raw[4] >> 16) & 0x0000FFFF);
}

// private data

// public functions
void net_TCP_init(void)
{
        net_TCP_internet_setTCPRxCallback(rxCallback);
}


// private functions
static void rxCallback(databuffer_basic_t *rxDataBuffer, ipv4_t sourceIP)
{
        static databuffer_basic_t rxDataBufferPayload;
        
        tcp_header_t *tcpHeader = (tcp_header_t *)&rxDataBuffer->data[0];
        
        uint16_t tcpPayloadOffset = tcp_getDataOffset(tcpHeader)
                                    * sizeof(uint32_t);
        uint16_t tcpPayloadLength = rxDataBuffer->length
                                    - tcp_getDataOffset(tcpHeader)
                                    * sizeof(uint32_t);
        
        databuffer_create(&rxDataBufferPayload,
                          &rxDataBuffer->data[tcpPayloadOffset],
                          tcpPayloadLength);
        
        char singleNumber[4];
        serialConsole_txString("\n\nrecv_TCP[");
        itoa(IPV4_getSingle(sourceIP, 0), singleNumber, 10);
        serialConsole_txString(singleNumber);
        serialConsole_txString(".");
        itoa(IPV4_getSingle(sourceIP, 1), singleNumber, 10);
        serialConsole_txString(singleNumber);
        serialConsole_txString(".");
        itoa(IPV4_getSingle(sourceIP, 2), singleNumber, 10);
        serialConsole_txString(singleNumber);
        serialConsole_txString(".");
        itoa(IPV4_getSingle(sourceIP, 3), singleNumber, 10);
        serialConsole_txString(singleNumber);
        serialConsole_txString("]:");
        itoa(tcp_getSourcePort(tcpHeader), singleNumber, 10);
        serialConsole_txString(singleNumber);
        serialConsole_txString("->");
        itoa(tcp_getDestinationPort(tcpHeader), singleNumber, 10);
        serialConsole_txString(singleNumber);
        serialConsole_txString(" = \'");
        serialConsole_txDatabuffer(&rxDataBufferPayload);
        serialConsole_txString("\'\n\n");
}


// interrupt service routines
