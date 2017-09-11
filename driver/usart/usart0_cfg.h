/**
 *******************************************************************************
 * @file        usart0_cfg.h
 * @version     0.0.1
 * @date        2017.09.11
 * @author      Michael Strosche (TheCross)
 * @brief       Config-file for the internal USART0-periphery.
 *
 * @since       V0.0.1, 2017.09.11:
 *                      -# Initiale Version (MS)
 *
 * @licence     The MIT License (MIT)
 *
 *              Copyright (c) 2017 Michael Strosche
 *
 *              Permission is hereby granted, free of charge, to any person
 *              obtaining a copy of this software and associated documentation
 *              files (the "Software"), to deal in the Software without
 *              restriction, including without limitation the rights to use,
 *              copy, modify, merge, publish, distribute, sublicense, and/or
 *              sell copies of the Software, and to permit persons to whom the
 *              Software is furnished to do so, subject to the following
 *              conditions:
 *
 *              The above copyright notice and this permission notice shall be
 *              included in all´copies or substantial portions of the Software.
 *
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

#define USART0_DATABITS_5 (0)
#define USART0_DATABITS_6 (1)
#define USART0_DATABITS_7 (2)
#define USART0_DATABITS_8 (3)
#define USART0_DATABITS_9 (7)
#define USART0_DATABITS USART0_DATABITS_8

#define USART0_PARITY_NONE  (0)
#define USART0_PARITY_EVEN  (2)
#define USART0_PARITY_ODD   (3)
#define USART0_PARITY USART0_PARITY_NONE

#define USART0_STOPBITS_1 (0)
#define USART0_STOPBITS_2 (1)
#define USART0_STOPBITS USART0_STOPBITS_1

#define USART0_USE2X

#endif /* _USART0_CFG_H_ */