/**
 *******************************************************************************
 * @file        PPP.h
 * @version     1.0
 * @date        2016.06.28
 * @author      M. Strosche
 * @brief       Header file of the PPP-Protocol-Stack.
 *              This module implements the PPP-Protocol-Stack for the
 *              Data-Link-Layer of the OSI-Model. It receives data from a serial
 *              connection and parses it byte-by-byte. Valid payload-data will
 *              be transfered to a higher level by executing a
 *              callback-function. The frame-format of the PPP-packets is HDLC.
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

#ifndef _NET_PPP_H_
#define _NET_PPP_H_

#include "..\\..\\system.h"
#include "..\\..\\utils\\databuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	NETPPP_IP	= 0x0021,			/* Internet Protocol */
	NETPPP_LCP	= 0xC021,			/* Link Control Protocol */
} net_PPP_protocol_e;

/**------------------------------------------------------------------------------
 *  @fn         net_PPP_init
 *  @brief      Initializes the PPP-Protocol-Stack on the Data-Link-Layer and the
 *              correspondig Physical-Layer-Module.
 *  @param      None.
 *  @return     None.
 *  @pre        None.
 *  @post       This module and all neccessary modules on lower levels of the
 *              OSI-Model are initialized.
 */
void net_PPP_init(void);

/**------------------------------------------------------------------------------
 *  @fn         net_PPP_loop
 *  @brief      Handles repetitive tasks within the PPP-Protocol-Stack.
 *  @param      None.
 *  @return     None.
 *  @pre        net_PPP_init has been called.
 *  @post       None.
 */
void net_PPP_loop(void);

/**------------------------------------------------------------------------------
 *  @fn         net_PPP_txDataBuffer
 *  @brief      Transmits the data for the specified protocol.
 *  @param      protocol:			Protocol identifier.
 *  @param      dataBufferChain:	Pointer to the first element of a dataBuffer-chain.
 *  @return     False if there is already a transmission in progress, true if a new transmission has been started.
 *  @pre        net_PPP_init has been called.
 *  @post       Process of transmission of a ppp-frame has been started if no other transmission is already in progress.
 */
void net_PPP_txDataBuffer(net_PPP_protocol_e protocol, databuffer_basic_t *dataBufferChain);

/**------------------------------------------------------------------------------
 *  @fn         net_PPP_isTxBusy
 *  @brief      Returns if a transmission is in progress.
 *  @param      None.
 *  @return     False if a transmission is in progress, true if idle.
 *  @pre        net_PPP_init has been called.
 *  @post       None.
 */
bool net_PPP_txIsBusy(void);

/**------------------------------------------------------------------------------
 *  @fn         net_PPP_setLCPRxCallback
 *  @brief      Sets the function that will be called when a new LCP-packet has
 *        been received.
 *  @param      rxCallback: Pointer to a function that handles the received
 *              data.
 *  @return     None.
 *  @pre        net_PPP_init has been called.
 *  @post       None.
 */
void net_PPP_setLCPRxCallback(void (*rxCallback)(databuffer_basic_t *rxDataBuffer));

/**------------------------------------------------------------------------------
 *  @fn         net_PPP_setIPRxCallback
 *  @brief      Sets the function that will be called when a new IP-packet has
 *  			been received.
 *  @param      rxCallback:	Pointer to a function that handles the received
 *  						data.
 *  @return     None.
 *  @pre        net_PPP_init has been called.
 *  @post       None.
 */
void net_PPP_setIPRxCallback(void (*rxCallback)(databuffer_basic_t *rxDataBuffer));

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _NET_PPP_H_ */
