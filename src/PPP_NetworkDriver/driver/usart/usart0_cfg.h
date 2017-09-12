/**
 *******************************************************************************
 * @file        usart0_cfg.h
 * @version     0.0.2
 * @date        2017.09.12
 * @author      Michael Strosche (TheCross)
 * @brief       Config-file for the internal USART0-periphery.
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

#ifndef _USART0_CFG_H_
#define _USART0_CFG_H_

/**
 *  This Define will specify the width of the data in Bits.                   @n
 *  Possible values are:                                                      @n
 *  0 -> 5 Databits                                                           @n
 *  1 -> 6 Databits                                                           @n
 *  2 -> 7 Databits                                                           @n
 *  3 -> 8 Databits                                                           @n
 *  7 -> 9 Databits
 */
#define USART0_DATABITS (8)

/**
 *  This Define will specify the Parity-Mode.                                 @n
 *  Possible values are:                                                      @n
 *  0 -> No Parity.                                                           @n
 *  2 -> Even Parity.                                                         @n
 *  3 -> Odd Parity.
 */
#define USART0_PARITY (0)

/**
 *  This Define will specify the number of Stop-Bits.                         @n
 *  Possible values are:                                                      @n
 *  0 -> 1 Stop-Bit.                                                          @n
 *  1 -> 2 Stop-Bits.
 */
#define USART0_STOPBITS (0)

/**
 *  Uncommented this Define to enable double-speed for the USART0-Periphery.
 */
#define USART0_USE2X

#endif /* _USART0_CFG_H_ */