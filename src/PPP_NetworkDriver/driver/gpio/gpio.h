/**
 *******************************************************************************
 * @file        gpio.h
 * @version     0.0.2
 * @date        2017.09.12
 * @author      Michael Strosche (TheCross)
 * @brief       This file defines helper to set/get individual gpio-pins.
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

#ifndef _GPIO_H_
#define _GPIO_H_

#include "..\\..\\system.h"

/**
 *  Sets a specific Pin as an Output-Pin.                                     @n
 *  The Parameters are:
 *      -# port: Letter of the Port (A, B, C, ...).
 *      -# pin: Number of the Pin (0, 1, 2, ...).
 *  @return     None.
 *  @pre        None.
 *  @post       The specified Pin has been configured as an Output-Pin.
 */
#define gpio_setPinAsOutput(_port_, _pin_) \
        CONCAT2(DDR, _port_)  |=  BV(_pin_)

/**
 *  Sets a specific Pin as an Input-Pin.                                      @n
 *  The Parameters are:
 *      -# port: Letter of the Port (A, B, C, ...).
 *      -# pin: Number of the Pin (0, 1, 2, ...).
 *  @return     None.
 *  @pre        None.
 *  @post       The specified Pin has been configured as an Input-Pin.
 */
#define gpio_setPinAsInput(_port_, _pin_) \
        CONCAT2(DDR, _port_)  &= ~BV(_pin_)

/**
 *  Sets the state of specific Pin as HIGH-Level.                             @n
 *  The Parameters are:
 *      -# port: Letter of the Port (A, B, C, ...).
 *      -# pin: Number of the Pin (0, 1, 2, ...).
 *  @return     None.
 *  @pre        None.
 *  @post       The specified Pin-State has been set to HIGH-Level.
 */
#define gpio_setPinStateHigh(_port_, _pin_) \
        CONCAT2(PORT, _port_) |=  BV(_pin_)

/**
 *  Sets the state of specific Pin as LOW-Level.                              @n
 *  The Parameters are:
 *      -# port: Letter of the Port (A, B, C, ...).
 *      -# pin: Number of the Pin (0, 1, 2, ...).
 *  @return     None.
 *  @pre        None.
 *  @post       The specified Pin-State has been set to LOW-Level.
 */
#define gpio_setPinStateLow(_port_, _pin_) \
        CONCAT2(PORT, _port_) &= ~BV(_pin_)

/**
 *  Returns the state of specific Pin.                                        @n
 *  The Parameters are:
 *      -# port: Letter of the Port (A, B, C, ...).
 *      -# pin: Number of the Pin (0, 1, 2, ...).
 *  @return     Pin-State (true == HIGH-Level).
 *  @pre        None.
 *  @post       None.
 */
#define gpio_getPinState(_port_, _pin_) \
        (CONCAT2(PIN, _port_) & BV(_pin_))

#endif /* _GPIO_H_ */