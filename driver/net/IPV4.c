/**
 *******************************************************************************
 * @file        IPV4.c
 * @version     1.0
 * @date        2016.06.28
 * @author      M. Strosche
 * @brief       Source file of the IPV4-Protocol-Stack.
 *              This module implements the IPV4-Protocol-Stack for the
 *              Internet-Layer of the OSI-Model.
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

#include "IPV4.h"
#include "IPV4_cfg.h"

#include "..\\..\\system.h"
#include "ipv4_common.h"
#include "..\\..\\utils\\databuffer.h"
#include "..\\..\\utils\\serialConsole.h"

#undef NET_IPV4_DATALINK_FILENAME
#define NET_IPV4_DATALINK_FILENAME            CONCAT1(NET_IPV4_DATALINK)
#define NET_IPV4_DATALINKINCLUDE              CREATEINCLUDEFILEWPATH(NET_IPV4_DATALINKPATH, NET_IPV4_DATALINK_FILENAME)
#include NET_IPV4_DATALINKINCLUDE

#define NET_IPV4_DATALINK_FUNPREFIX           CONCAT2(net_, NET_IPV4_DATALINK)
#define net_IPV4_datalink_setIPRxCallback     CONCAT2(NET_IPV4_DATALINK_FUNPREFIX, _setIPRxCallback)


// type-definitions


// private function prototypes
static void rxCallback(databuffer_basic_t *rxDataBuffer);
static void rxCallback_DUMMY(databuffer_basic_t *rxDataBuffer, ipv4_t sourceIP);

// private data
// RX-Callback-Functions
static void (*rxCallback_UDP)(databuffer_basic_t *rxDataBuffer, ipv4_t sourceIP) = rxCallback_DUMMY;
static void (*rxCallback_TCP)(databuffer_basic_t *rxDataBuffer, ipv4_t sourceIP) = rxCallback_DUMMY;
static ipv4_t localIP;

// public functions
void net_IPV4_init(void)
{
	net_IPV4_datalink_setIPRxCallback(rxCallback);

	uint8_t ip[4] = {NET_IPV4_IP_DEFAULT};
	localIP = IPV4_create(ip);
}

void net_IPV4_setLocalIP(uint8_t ip[4])
{
	localIP = IPV4_create(ip);
}

void net_IPV4_setUDPRxCallback(void (*rxCallback)(databuffer_basic_t *rxDataBuffer, ipv4_t sourceIP))
{
	if (rxCallback != NULL)
		rxCallback_UDP = rxCallback;
}

void net_IPV4_setTCPRxCallback(void (*rxCallback)(databuffer_basic_t *rxDataBuffer, ipv4_t sourceIP))
{
	if (rxCallback != NULL)
		rxCallback_TCP = rxCallback;
}


// private functions
static void rxCallback(databuffer_basic_t *rxDataBuffer)
{
	static databuffer_basic_t rxDataBufferPayload;
	
	ipv4_t sourceIP;
	ipv4_header_t *ipHeader = (ipv4_header_t *)&rxDataBuffer->data[0];

	// check version
	if (IPV4_header_getVersion(ipHeader) == 4) {
		// check destination IP
		if (IPV4_header_getDestinationIPAddress(ipHeader).raw == localIP.raw)  {
			sourceIP = IPV4_header_getSourceIPAddress(ipHeader);

			uint8_t payloadOffset = IPV4_header_getIHL(ipHeader) * 4;
			uint16_t payloadLength = IPV4_header_getTotalLength(ipHeader) - payloadOffset;

			databuffer_create(&rxDataBufferPayload, &rxDataBuffer->data[payloadOffset], payloadLength);

			switch (IPV4_header_getProtocol(ipHeader)) {
				case IP_PROTOCOL_UDP:
					rxCallback_UDP(&rxDataBufferPayload, sourceIP);
					break;
					
				case IP_PROTOCOL_TCP:
					rxCallback_TCP(&rxDataBufferPayload, sourceIP);
					break;
					
				default:
					rxCallback_DUMMY(&rxDataBufferPayload, sourceIP);
					break;
			}
		} else {
			serialConsole_txString("not my IP\n");
		}
	} else {
		serialConsole_txString("IP.version != 4\n");
	}
}

static void rxCallback_DUMMY(databuffer_basic_t *rxDataBuffer, ipv4_t sourceIP)
{
	char singleNumber[4];
	serialConsole_txString("\n\nrecv_IP_?[");
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
	serialConsole_txString("] = \'");
	serialConsole_txDatabuffer(rxDataBuffer);
	serialConsole_txString("\'\n\n");
}


// interrupt service routines
