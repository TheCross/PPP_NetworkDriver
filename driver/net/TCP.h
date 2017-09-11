/**
 *******************************************************************************
 * @file        TCP.h
 * @version     1.0
 * @date        2016.06.28
 * @author      M. Strosche
 * @brief       Header file of the TCP-Protocol-Stack.
 *              This module implements the TCP-Protocol-Stack for the
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

#ifndef _NET_TCP_H_
#define _NET_TCP_H_

#include "..\\..\\system.h"
#include "..\\..\\utils\\databuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**------------------------------------------------------------------------------
 *  @fn         net_TCP_init
 *  @brief      Initializes the TCP-Protocol-Stack on the Transport-Layer and the
 *              correspondig Internet-Module.
 *  @param      None.
 *  @return     None.
 *  @pre        All neccessary modules on lower levels of the OSI-Model are
 *				initialized.
 *  @post       This module is initialized.
 */
void net_TCP_init(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _NET_TCP_H_ */
