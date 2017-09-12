/**
 *******************************************************************************
 * @file        IPV4_cfg.h
 * @version     1.0
 * @date        2016.06.28
 * @author      M. Strosche
 * @brief       Config file of the IPV4-Protocol-Stack.
 *
 * @since       V1.0, 2016.06.28:
 *              -# Initiale Version (MS)
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

#ifndef _NET_IPV4_CFG_H_
#define _NET_IPV4_CFG_H_

/**
 *  Path of the DataLink-Module.
 */
#define NET_IPV4_DATALINKPATH           .

/**
 *  Type of the DataLink-Module.                                              @n
 *  Possible values are:                                                      @n
 *  PPP
 */
#define NET_IPV4_DATALINK               PPP

/**
 *  Default IP-Address (comma-separated).
 */
#define NET_IPV4_IP_DEFAULT             192, 168, 1, 2

#endif /* _NET_IPV4_CFG_H_ */
