/**
 *******************************************************************************
 * @file        TCP.h
 * @version     0.0.2
 * @date        2017.09.12
 * @author      M. Strosche
 * @brief       Header file of the TCP-Protocol-Stack.
 *              This module implements the TCP-Protocol-Stack for the
 *              Transport-Layer of the OSI-Model.
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

#ifndef _NET_TCP_H_
#define _NET_TCP_H_

#include "..\\..\\system.h"
#include "..\\..\\utils\\databuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Initializes the TCP-Protocol-Stack on the Transport-Layer and the
 *  correspondig Internet-Module.
 *  @return     None.
 *  @pre        All neccessary modules on lower levels of the OSI-Model are
 *              initialized.
 *  @post       This module is initialized.
 */
void net_TCP_init(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _NET_TCP_H_ */
