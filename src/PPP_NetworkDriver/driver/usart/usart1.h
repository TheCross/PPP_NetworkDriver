/**
 *******************************************************************************
 * @file        usart1.h
 * @version     0.0.2
 * @date        2017.09.12
 * @author      Michael Strosche (TheCross)
 * @brief       Header-file for the internal USART1-periphery.
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

#ifndef _USART1_H_
#define _USART1_H_

#include "..\\..\\system.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Initializes the internal periphery of the USART1.
 *  @param      baudrate: Baudrate.
 *  @return     None.
 *  @pre        None.
 *  @post       USART1 has been initialized.
 */
void usart1_init(uint32_t baudrate);

/**
 *  Sets the Callback-Function that will be called each time a new Byte has been
 *  received.
 *  @param      callback: Callback-Function of type void:uint8.
 *  @return     None.
 *  @pre        None.
 *  @post       Callback-Function has been set.
 */
void usart1_setRxFinishedCallback(void (*callback)(uint8_t b));

/**
 *  Sets the Callback-Function that will be called each time a Byte has been
 *  transmitted.
 *  @param      callback: Callback-Function of type void:void.
 *  @return     None.
 *  @pre        None.
 *  @post       Callback-Function has been set.
 */
void usart1_setTxFinishedCallback(void (*callback)(void));

/**
 *  Starts the transmission of a single Byte.                                 @n
 *  After the transmission the set Callback-Function (TX) will be called.
 *  @param      b: Byte to transmit.
 *  @return     None.
 *  @pre        The function usart1_init had been called.
 *  @post       The transmission of the Byte has started.
 */
inline void usart1_txByte(uint8_t b)
{
        UDR1 = b;
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _USART1_H_ */
