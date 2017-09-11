/**
 *******************************************************************************
 * @file        UDP.c
 * @version     1.0
 * @date        2016.06.28
 * @author      M. Strosche
 * @brief       Source file of the UDP-Protocol-Stack.
 *              This module implements the UDP-Protocol-Stack for the
 *              Transport-Layer of the OSI-Model.
 *
 * @since       V1.0, 2016.06.28:
 *              -# Initiale Version (MS)
 *
 * @copyright   2017 Michael Strosche.
 *              All rights are reserved.
 *
 * @licence     ...
 *
 *******************************************************************************
 */

#include "UDP.h"
#include "UDP_cfg.h"

#include "..\\..\\system.h"
#include "..\\..\\utils\\databuffer.h"
#include "..\\..\\utils\\serialConsole.h"

#undef NET_UDP_INTERNET_FILENAME
#define NET_UDP_INTERNET_FILENAME           CONCAT1(NET_UDP_INTERNET)
#define NET_UDP_INTERNETINCLUDE             CREATEINCLUDEFILEWPATH(NET_UDP_INTERNETPATH, NET_UDP_INTERNET_FILENAME)
#include NET_UDP_INTERNETINCLUDE

#undef NET_UDP_INTERNET_FILENAME
#define NET_UDP_INTERNET_FILENAME           CONCAT2(NET_UDP_INTERNET, _common)
#define NET_UDP_INTERNETINCLUDECOMMON       CREATEINCLUDEFILEWPATH(NET_UDP_INTERNETPATH, NET_UDP_INTERNET_FILENAME)
#include NET_UDP_INTERNETINCLUDECOMMON

#define NET_UDP_INTERNET_FUNPREFIX          CONCAT2(net_, NET_UDP_INTERNET)
#define net_UDP_internet_setUDPRxCallback   CONCAT2(NET_UDP_INTERNET_FUNPREFIX, _setUDPRxCallback)

// type-definitions
typedef struct {
	uint32_t raw[2];
} udp_header_t;


// private function prototypes
static void rxCallback(databuffer_basic_t *rxDataBuffer, ipv4_t sourceIP);
inline uint16_t udp_getSourcePort(udp_header_t *udp_header)
{
	return (uint16_t)((udp_header->raw[0] >> 0) & 0x0000FFFF);
}

inline uint16_t udp_getDestinationPort(udp_header_t *udp_header)
{
	return (uint16_t)((udp_header->raw[0] >> 16) & 0x0000FFFF);
}

inline uint16_t udp_getLength(udp_header_t *udp_header)
{
	return (uint16_t)((udp_header->raw[1] >> 0) & 0x0000FFFF);
}

inline uint16_t udp_getPayloadLength(udp_header_t *udp_header)
{
	return udp_getLength(udp_header) - sizeof(udp_header_t);
}

inline uint16_t udp_getCRC(udp_header_t *udp_header)
{
	return (uint16_t)((udp_header->raw[1] >> 16) & 0x0000FFFF);
}

// private data


// public functions
void net_UDP_init(void)
{
	net_UDP_internet_setUDPRxCallback(rxCallback);
}


// private functions
static void rxCallback(databuffer_basic_t *rxDataBuffer, ipv4_t sourceIP)
{
	static databuffer_basic_t rxDataBufferPayload;
	
	udp_header_t *udpHeader = (udp_header_t *)&rxDataBuffer->data[0];
	
	databuffer_create(&rxDataBufferPayload, &rxDataBuffer->data[sizeof(udp_header_t)], udp_getPayloadLength(udpHeader));
	
	char singleNumber[8];
	serialConsole_txString("\n\nrecv_UDP[");
	itoa(IPV4_getSingle(sourceIP, 0), singleNumber, 10);	serialConsole_txString(singleNumber);	serialConsole_txString(".");
	itoa(IPV4_getSingle(sourceIP, 1), singleNumber, 10);	serialConsole_txString(singleNumber);	serialConsole_txString(".");
	itoa(IPV4_getSingle(sourceIP, 2), singleNumber, 10);	serialConsole_txString(singleNumber);	serialConsole_txString(".");
	itoa(IPV4_getSingle(sourceIP, 3), singleNumber, 10);	serialConsole_txString(singleNumber);
	serialConsole_txString("]:");
	itoa(udp_getSourcePort(udpHeader), singleNumber, 10);	serialConsole_txString(singleNumber);
	serialConsole_txString("->");
	itoa(udp_getDestinationPort(udpHeader), singleNumber, 10);	serialConsole_txString(singleNumber);
	serialConsole_txString(" = \'");
	serialConsole_txDatabuffer(&rxDataBufferPayload);
	serialConsole_txString("\'\n\n");
}


// interrupt service routines
