/**
 *******************************************************************************
 * @file        myUtils.h
 * @version     0.0.1
 * @date        2017.09.11
 * @author      Michael Strosche (TheCross)
 * @brief       This file defines helper-functions.
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

#ifndef _MYUTILS_H_
#define _MYUTILS_H_

#define CONCAT1_(_a_)                     _a_
#define CONCAT1(_a_)                      CONCAT1_(_a_)

#define CONCAT2_(_a_, _b_)                _a_##_b_
#define CONCAT2(_a_, _b_)                 CONCAT2_(_a_, _b_)

#define CONCAT3_(_a_, _b_, _c_)           _a_##_b_##_c_
#define CONCAT3(_a_, _b_, _c_)            CONCAT3_(_a_, _b_, _c_)

#define CONCAT4_(_a_, _b_, _c_, _d_)     _a_##_b_##_c_##_d_
#define CONCAT4(_a_, _b_, _c_, _d_)       CONCAT3_(_a_, _b_, _c_, _d_)

#define STRINGIFY_(_a_)                   #_a_
#define STRINGIFY(_a_)                    STRINGIFY_(_a_)

#define CREATEINCLUDEFILE(_a_)            STRINGIFY(_a_.h)
#define CREATEINCLUDEFILEWPATH(_p_, _a_)  CREATEINCLUDEFILE(CONCAT1(_p_)\_a_)

#define SHIFT(_a_, _b_)                   (_a_ << _b_)
#define BV(_a_)                           (SHIFT(1, _a_))

#ifndef min
        #define min(_a_, _b_)                      \
                ({ __typeof__ (_a_) __a__ = (_a_); \
                   __typeof__ (_b_) __b__ = (_b_); \
                   __a__ < __b__ ? __a__ : __b__;  \
                })
#endif /* max */

#ifndef max
        #define max(_a_, _b_)                      \
                ({ __typeof__ (_a_) __a__ = (_a_); \
                   __typeof__ (_b_) __b__ = (_b_); \
                   __a__ > __b__ ? __a__ : __b__;  \
                })
#endif /* max */

#endif /* _MYUTILS_H_ */
