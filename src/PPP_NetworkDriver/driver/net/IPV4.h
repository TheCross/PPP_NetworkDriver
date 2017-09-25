/**
 *******************************************************************************
 * @file        IPV4.h
 * @version     0.0.2
 * @date        2017.09.25
 * @author      M. Strosche
 * @brief       Header file of the IPV4-Protocol-Stack.
 *              This module implements the IPV4-Protocol-Stack for the
 *              Internet-Layer of the OSI-Model.
 *
 * @since       V0.0.2, 2017.09.25:
 *                      -# No typedefs for struct and enum. (MS)
 *
 * @since       V0.0.1, 2017.09.11:
 *                      -# Initiale Version (MS)
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

#ifndef _NET_IPV4_H_
#define _NET_IPV4_H_

#include "..\\..\\system.h"
#include "ipv4_common.h"
#include "..\\..\\utils\\databuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This enum specifies the different protocols that can be handled by the IPV4-
 * Module.
 */
enum ipv4_protocol_e {
        IP_PROTOCOL_UDP = 0x11,
        IP_PROTOCOL_TCP = 0x06
};

/**
 *  Initializes the IPV4-Protocol-Stack on the Internet-Layer and the
 *  correspondig Data-Link-Layer-Module.
 *  @return     None.
 *  @pre        None.
 *  @post       This module and all neccessary modules on lower levels of the
 *              OSI-Model are initialized.
 */
void net_IPV4_init(void);

/**
 *  Sets the local IP-address.
 *  @param      ip: new local IP-address.
 *  @return     None.
 *  @pre        net_IP_init has been called.
 *  @post       Local IP-address is set to the specified value.
 */
void net_IPV4_setLocalIP(uint8_t ip[4]);

/**
 *  Sets the function that will be called when a new UDP-packet has been
 *  received.
 *  @param      rxCallback: Pointer to a function that handles the received
 *                          data.
 *  @return     None.
 *  @pre        net_PPP_init has been called.
 *  @post       None.
 */
void net_IPV4_setUDPRxCallback(void (*rxCallback)(struct databuffer_basic_t *rxDataBuffer,
                                                  ipv4_t sourceIP));

/**
 *  Sets the function that will be called when a new TCP-packet has been
 *  received.
 *  @param      rxCallback: Pointer to a function that handles the received
 *                          data.
 *  @return     None.
 *  @pre        net_PPP_init has been called.
 *  @post       None.
 */
void net_IPV4_setTCPRxCallback(void (*rxCallback)(struct databuffer_basic_t *rxDataBuffer,
                                                  ipv4_t sourceIP));

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _NET_IPV4_H_ */
