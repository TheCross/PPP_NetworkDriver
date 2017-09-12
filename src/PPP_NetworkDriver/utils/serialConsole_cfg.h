/**
 *******************************************************************************
 * @file        serialConsole_cfg.h
 * @version     0.0.2
 * @date        2017.09.12
 * @author      Michael Strosche (TheCross)
 * @brief       Config-file to handle input/output of serial data (strings,
 *              etc.).
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

#ifndef _SERIALCONSOLE_CFG_H_
#define _SERIALCONSOLE_CFG_H_

/**
 *  Size of the RX-Buffer in Bytes.
 */
#define SERIALCONSOLE_RX_BUFFERSIZE (64)

/**
 *  Size of the TX-Buffer in Bytes.
 */
#define SERIALCONSOLE_TX_BUFFERSIZE (64)

/**
 *  Baudrate of the UART-Driver.
 */
#define SERIALCONSOLE_BAUDRATE      (115200UL)

/**
 *  Path of the UART-Driver.
 */
#define SERIALCONSOLE_UARTPATH      ..\\driver\\usart

/**
 *  Type of the UART-Driver.                                                  @n
 *  Possible values are:                                                      @n
 *  uart                                                                      @n
 *  usart
 */
#define SERIALCONSOLE_UARTTYPE      usart

/**
 *  Number of the UART-Driver.
 */
#define SERIALCONSOLE_UARTNUMBER    0

#endif /* _SERIALCONSOLE_CFG_H_ */
