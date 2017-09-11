/**
 *******************************************************************************
 * @file        IPV4.h
 * @version     1.0
 * @date        2016.06.28
 * @author      M. Strosche
 * @brief       Header file of the IPV4-Protocol-Stack.
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

#ifndef _NET_IPV4_H_
#define _NET_IPV4_H_

#include "..\\..\\system.h"
#include "ipv4_common.h"
#include "..\\..\\utils\\databuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	IP_PROTOCOL_UDP = 0x11,
	IP_PROTOCOL_TCP = 0x06
} ipv4_protocol_e;

/**------------------------------------------------------------------------------
 *  @fn         net_IPV4_init
 *  @brief      Initializes the IPV4-Protocol-Stack on the Internet-Layer and the
 *              correspondig Data-Link-Layer-Module.
 *  @param      None.
 *  @return     None.
 *  @pre        None.
 *  @post       This module and all neccessary modules on lower levels of the
 *              OSI-Model are initialized.
 */
void net_IPV4_init(void);

/**------------------------------------------------------------------------------
 *  @fn         net_IPV4_setLocalIP
 *  @brief      Sets the local IP-address.
 *  @param      ip:	new local IP-address.
 *  @return     None.
 *  @pre        net_IP_init has been called.
 *  @post       Local IP-address is set to the specified value.
 */
void net_IPV4_setLocalIP(uint8_t ip[4]);

/**------------------------------------------------------------------------------
 *  @fn         net_IPV4_setUDPRxCallback
 *  @brief      Sets the function that will be called when a new UDP-packet has
 *  			been received.
 *  @param      rxCallback:	Pointer to a function that handles the received
 *  						data.
 *  @return     None.
 *  @pre        net_PPP_init has been called.
 *  @post       None.
 */
void net_IPV4_setUDPRxCallback(void (*rxCallback)(databuffer_basic_t *rxDataBuffer, ipv4_t sourceIP));

/**------------------------------------------------------------------------------
 *  @fn         net_IPV4_setTCPRxCallback
 *  @brief      Sets the function that will be called when a new TCP-packet has
 *  			been received.
 *  @param      rxCallback:	Pointer to a function that handles the received
 *  						data.
 *  @return     None.
 *  @pre        net_PPP_init has been called.
 *  @post       None.
 */
void net_IPV4_setTCPRxCallback(void (*rxCallback)(databuffer_basic_t *rxDataBuffer, ipv4_t sourceIP));

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _NET_IPV4_H_ */
