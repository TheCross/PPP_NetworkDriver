/**
 *******************************************************************************
 * @file        PPP_cfg.h
 * @version     1.0
 * @date        2016.06.28
 * @author      M. Strosche
 * @brief       Config file of the PPP-Protocol-Stack.
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

#ifndef _NET_PPP_CFG_H_
#define _NET_PPP_CFG_H_

#define NET_PPP_BAUDRATE                (19200UL)

#define NET_PPP_UARTPATH		        ..\\usart
#define NET_PPP_UARTTYPE                usart
#define NET_PPP_UARTNUMBER              1

#define NET_PPP_MTU_MAX                 (576)

// number of buffered rx-packets
#define NET_PPP_RX_PACKET_BUFFER_SIZE   2

//#define NET_PPP_RELAY_INSTREAM

#endif /* _NET_PPP_CFG_H_ */
