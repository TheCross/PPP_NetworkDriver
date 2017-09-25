/**
 *******************************************************************************
 * @file        myUtils.h
 * @version     0.0.1
 * @date        2017.09.21
 * @author      Michael Strosche (TheCross)
 * @brief       This file defines gcc specific helper-functions.
 *
 * @since       V0.0.1, 2017.09.21:
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

#ifndef _GCCUTILS_H_
#define _GCCUTILS_H_

#define GCC_VERSION     \
        (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#ifndef min
        /**
         *  This Macro defines the Minimum-Function if not implemented.
         */
        #define min(_a_, _b_)                      \
                ({ __typeof__ (_a_) __a__ = (_a_); \
                   __typeof__ (_b_) __b__ = (_b_); \
                   __a__ < __b__ ? __a__ : __b__;  \
                })
#endif /* max */

#ifndef max
        /**
         *  This Macro defines the Maximum-Function if not implemented.
         */
        #define max(_a_, _b_)                      \
                ({ __typeof__ (_a_) __a__ = (_a_); \
                   __typeof__ (_b_) __b__ = (_b_); \
                   __a__ > __b__ ? __a__ : __b__;  \
                })
#endif /* max */

/**
 *  This Macro will check if two variables are of the same type.
 */
#define __same_type(_a_, _b_)   \
        __builtin_types_compatible_p(__typeof__(_a_), __typeof__(_b_))

/**
 *  This Macro will define the byte order.
 */
#define BYTEORDER               __BYTE_ORDER__

/**
 *  This Macro will define the value for little-enadian.
 */
#define BYTEORDER_LITTLE        __ORDER_LITTLE_ENDIAN__

/**
 *  This Macro will define the value for big-enadian.
 */
#define BYTEORDER_BIG           __ORDER_BIG_ENDIAN__

/**
 *  This Macro will define the value for pdp-enadian.
 */
#define BYTEORDER_PDP           __ORDER_PDP_ENDIAN__

#endif /* _GCCUTILS_H_ */
