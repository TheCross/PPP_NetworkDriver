/**
 *******************************************************************************
 * @file        UDP.c
 * @version     0.0.3
 * @date        2017.09.25
 * @author      M. Strosche
 * @brief       Source file of the UDP-Protocol-Stack.
 *              This module implements the UDP-Protocol-Stack for the
 *              Transport-Layer of the OSI-Model.
 *
 * @since       V0.0.3, 2017.09.25:
 *                      -# Changed from inline to macro. (MS)
 *                      -# No typedefs for struct and enum. (MS)
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

#include "UDP.h"
#include "UDP_cfg.h"

#include "..\\..\\system.h"
#include "..\\..\\utils\\databuffer.h"
#include "..\\..\\utils\\serialConsole.h"

#undef NET_UDP_INTERNET_FILENAME
#define NET_UDP_INTERNET_FILENAME \
        CONCAT1(NET_UDP_INTERNET)
#define NET_UDP_INTERNETINCLUDE \
        CREATEINCLUDEFILEWPATH(NET_UDP_INTERNETPATH, NET_UDP_INTERNET_FILENAME)
#include NET_UDP_INTERNETINCLUDE

#undef NET_UDP_INTERNET_FILENAME
#define NET_UDP_INTERNET_FILENAME \
        CONCAT2(NET_UDP_INTERNET, _common)
#define NET_UDP_INTERNETINCLUDECOMMON \
        CREATEINCLUDEFILEWPATH(NET_UDP_INTERNETPATH, NET_UDP_INTERNET_FILENAME)
#include NET_UDP_INTERNETINCLUDECOMMON

#define NET_UDP_INTERNET_FUNPREFIX \
        CONCAT2(net_, NET_UDP_INTERNET)
#define net_UDP_internet_setUDPRxCallback \
        CONCAT2(NET_UDP_INTERNET_FUNPREFIX, _setUDPRxCallback)

// type-definitions
struct udp_header_t {
        uint32_t raw[2];
};


// private function prototypes
static void rxCallback(struct databuffer_basic_t *rxDataBuffer, ipv4_t sourceIP);
#define udp_getSourcePort(_udp_header_) \
        ((uint16_t)((_udp_header_->raw[0] >> 0) & 0x0000FFFF))

#define udp_getDestinationPort(_udp_header_)    \
        ((uint16_t)((_udp_header_->raw[0] >> 16) & 0x0000FFFF))

#define udp_getLength(_udp_header_)     \
        ((uint16_t)((_udp_header_->raw[1] >> 0) & 0x0000FFFF))

#define udp_getPayloadLength(_udp_header_)       \
        (udp_getLength(_udp_header_) - sizeof(struct udp_header_t))

#define udp_getCRC(_udp_header_)        \
        ((uint16_t)((_udp_header_->raw[1] >> 16) & 0x0000FFFF))

// private data


// public functions
void net_UDP_init(void)
{
        net_UDP_internet_setUDPRxCallback(rxCallback);
}


// private functions
static void rxCallback(struct databuffer_basic_t *rxDataBuffer, ipv4_t sourceIP)
{
        static struct databuffer_basic_t rxDataBufferPayload;
        
        struct udp_header_t *udpHeader = (struct udp_header_t *)&rxDataBuffer->data[0];
        
        databuffer_create(&rxDataBufferPayload,
                          &rxDataBuffer->data[sizeof(struct udp_header_t)],
                          udp_getPayloadLength(udpHeader));
        
        char singleNumber[8];
        serialConsole_txString("\n\nrecv_UDP[");
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
        itoa(udp_getSourcePort(udpHeader), singleNumber, 10);
        serialConsole_txString(singleNumber);
        serialConsole_txString("->");
        itoa(udp_getDestinationPort(udpHeader), singleNumber, 10);
        serialConsole_txString(singleNumber);
        serialConsole_txString(" = \'");
        serialConsole_txDatabuffer(&rxDataBufferPayload);
        serialConsole_txString("\'\n\n");
}


// interrupt service routines
