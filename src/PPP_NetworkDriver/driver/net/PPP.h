/**
 *******************************************************************************
 * @file        PPP.h
 * @version     0.0.3
 * @date        2017.09.25
 * @author      M. Strosche
 * @brief       Header file of the PPP-Protocol-Stack.
 *              This module implements the PPP-Protocol-Stack for the
 *              Data-Link-Layer of the OSI-Model. It receives data from a serial
 *              connection and parses it byte-by-byte. Valid payload-data will
 *              be transfered to a higher level by executing a
 *              callback-function. The frame-format of the PPP-packets is HDLC.
 *
 * @since       V0.0.3, 2017.09.25:
 *                      -# Added net_PPP_setMtuSize. (MS)
 *                      -# No typedefs for struct and enum. (MS)
 *                      -# Added net_PPP_getState. (MS)
 *                      -# Moved enum net_PPP_state_e from c to h. (MS)
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

#ifndef _NET_PPP_H_
#define _NET_PPP_H_

#include "..\\..\\system.h"
#include "..\\..\\utils\\databuffer.h"
#include ".\\PPP_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This enum specifies the different protocols that can be handled by the PPP-
 * Module.
 */
enum net_PPP_protocol_e {
        NETPPP_IP  = 0x0021,                   /* Internet Protocol */
        NETPPP_LCP = 0xC021,                   /* Link Control Protocol */
};

enum net_PPP_state_e {
        PPPState_Dead,
        PPPState_Establish,
        PPPState_Authenticate,
        PPPState_Network,
        PPPState_Terminate,
};

/**
 *  Initializes the PPP-Protocol-Stack on the Data-Link-Layer and the
 *  correspondig Physical-Layer-Module.
 *  @return     None.
 *  @pre        None.
 *  @post       This module and all neccessary modules on lower levels of the
 *              OSI-Model are initialized.
 */
void net_PPP_init(void);

/**
 *  Handles repetitive tasks within the PPP-Protocol-Stack.
 *  @return     None.
 *  @pre        net_PPP_init has been called.
 *  @post       None.
 */
void net_PPP_loop(void);

/**
 *  Transmits the data for the specified protocol.
 *  @param      protocol: Protocol identifier.
 *  @param      dataBufferChain: Pointer to the first element of a
 *                               DataBuffer-Chain.
 *  @return     False if there is already a transmission in progress, true if a
 *              new transmission has been started.
 *  @pre        net_PPP_init has been called.
 *  @post       Process of transmission of a ppp-frame has been started if no
 *              other transmission is already in progress.
 */
void net_PPP_txDataBuffer(enum net_PPP_protocol_e protocol,
                          struct databuffer_basic_t *dataBufferChain);

/**
 *  Returns if a transmission is in progress.
 *  @return     False if a transmission is in progress, true if idle.
 *  @pre        net_PPP_init has been called.
 *  @post       None.
 */
bool net_PPP_txIsBusy(void);

/**
 *  Sets the function that will be called when a new LCP-packet has been
 *  received.
 *  @param      rxCallback: Pointer to a function that handles the received
 *              data.
 *  @return     None.
 *  @pre        net_PPP_init has been called.
 *  @post       None.
 */
void net_PPP_setLCPRxCallback(void (*rxCallback)(struct databuffer_basic_t *rxDataBuffer));

/**
 *  Sets the function that will be called when a new IP-packet has been
 *  received.
 *  @param      rxCallback:     Pointer to a function that handles the received
 *                                              data.
 *  @return     None.
 *  @pre        net_PPP_init has been called.
 *  @post       None.
 */
void net_PPP_setIPRxCallback(void (*rxCallback)(struct databuffer_basic_t *rxDataBuffer));

/**
 *  Sets the new MTU-Size.
 *  @param      newMtuSize: New MTU-Size.
 *  @return     True, if the new size ist set, otherwise false.
 *  @pre        net_PPP_init has been called.
 *  @post       If the MTU-Size is less than the default MTU-Size the new size
 *              will be set and true will be returned. Otherwise the default
 *              MTU-Size will be set and false will be returned.
 */
bool net_PPP_setMtuSize(uint16_t newMtuSize);

/**
 *  Sets the current state of the PPP-Connection.
 *  @param      newState: New state.
 *  @return     None.
 *  @pre        net_PPP_init has been called.
 *  @post       New state has been set.
 */
void net_PPP_setState(enum net_PPP_state_e newState);

/**
 *  Returns the current state of the PPP-Connection.
 *  @return     Current state of the PPP-Connection.
 *  @pre        net_PPP_init has been called.
 *  @post       None.
 */
enum net_PPP_state_e net_PPP_getState(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _NET_PPP_H_ */
