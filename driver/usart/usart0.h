/**
 *******************************************************************************
 * @file        usart0.h
 * @version     0.0.1
 * @date        2017.09.11
 * @author      Michael Strosche (TheCross)
 * @brief       Header-file for the internal USART0-periphery.
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

#ifndef _USART0_H_
#define _USART0_H_

#include "..\\..\\system.h"

#ifdef __cplusplus
extern "C" {
#endif

void usart0_init(uint32_t baudrate);

void usart0_setRxFinishedCallback(void (*callback)(uint8_t b));
void usart0_setTxFinishedCallback(void (*callback)(void));

inline void usart0_txByte(uint8_t b)
{
        UDR0 = b;
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _USART0_H_ */
