/**
 *******************************************************************************
 * @file        LCP.h
 * @version     1.0
 * @date        2016.06.28
 * @author      M. Strosche
 * @brief       Header file of the LCP-Protocol-Stack.
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

#ifndef _NET_LCP_H_
#define _NET_LCP_H_

#include "..\\..\\system.h"
#include "..\\..\\utils\\databuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**------------------------------------------------------------------------------
 *  @fn         net_LCP_init
 *  @brief      Initializes the LCP-Protocol-Stack on the Data-Link-Layer.
 *  @param      None.
 *  @return     None.
 *  @pre        None.
 *  @post       This module is initialized.
 */
void net_LCP_init(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _NET_LCP_H_ */
