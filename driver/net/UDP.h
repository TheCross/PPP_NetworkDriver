/**
 *******************************************************************************
 * @file        UDP.h
 * @version     1.0
 * @date        2016.06.28
 * @author      M. Strosche
 * @brief       Header file of the UDP-Protocol-Stack.
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

#ifndef _NET_UDP_H_
#define _NET_UDP_H_

#include "..\\..\\system.h"
#include "..\\..\\utils\\databuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**------------------------------------------------------------------------------
 *  @fn         net_UDP_init
 *  @brief      Initializes the UDP-Protocol-Stack on the Transport-Layer and the
 *              correspondig Internet-Module.
 *  @param      None.
 *  @return     None.
 *  @pre        All neccessary modules on lower levels of the OSI-Model are
 *				initialized.
 *  @post       This module is initialized.
 */
void net_UDP_init(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _NET_UDP_H_ */
