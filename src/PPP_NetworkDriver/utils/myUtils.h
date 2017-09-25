/**
 *******************************************************************************
 * @file        myUtils.h
 * @version     0.0.3
 * @date        2017.09.25
 * @author      Michael Strosche (TheCross)
 * @brief       This file defines helper-functions.
 *
 * @since       V0.0.3, 2017.09.25:
 *                      -# Added aditional helper-functions. (MS)
 *                      -# Moved gcc specific stuff in separate file. (MS)
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

#ifndef _MYUTILS_H_
#define _MYUTILS_H_

/**
 *  This Macro will expand the refered expression.                            @n
 *  Helper-Macro for CONCAT1.
 *  @see CONCAT1
 */
#define CONCAT1_(_a_)   \
        _a_
/**
 *  This Macro will expand the refered expression.
 */
#define CONCAT1(_a_)    \
        CONCAT1_(_a_)

/**
 *  This Macro will expand the refered expression and concatenate them.       @n
 *  Helper-Macro for CONCAT2.
 *  @see CONCAT2
 */
#define CONCAT2_(_a_, _b_)      \
        _a_##_b_
/**
 *  This Macro will expand the refered expression and concatenate them.
 */
#define CONCAT2(_a_, _b_)       \
        CONCAT2_(_a_, _b_)

/**
 *  This Macro will expand the refered expression and concatenate them.       @n
 *  Helper-Macro for CONCAT3.
 *  @see CONCAT3
 */
#define CONCAT3_(_a_, _b_, _c_) \
        _a_##_b_##_c_
/**
 *  This Macro will expand the refered expression and concatenate them.
 */
#define CONCAT3(_a_, _b_, _c_)  \
        CONCAT3_(_a_, _b_, _c_)

/**
 *  This Macro will expand the refered expression and concatenate them.       @n
 *  Helper-Macro for CONCAT4.
 *  @see CONCAT4
 */
#define CONCAT4_(_a_, _b_, _c_, _d_)    \
        _a_##_b_##_c_##_d_
/**
 *  This Macro will expand the refered expression and concatenate them.
 */
#define CONCAT4(_a_, _b_, _c_, _d_)     \
        CONCAT3_(_a_, _b_, _c_, _d_)

/**
 *  This Macro will convert the refered expression into a string.             @n
 *  Helper-Macro for STRINGIFY.
 *  @see STRINGIFY
 */
#define STRINGIFY_(_a_) \
        #_a_
/**
 *  This Macro will convert the refered expression into a string.
 */
#define STRINGIFY(_a_)  \
        STRINGIFY_(_a_)

/**
 *  This Macro will convert the refered expression into a string and append ".h"
 *  to use it as a include-filename.
 */
#define CREATEINCLUDEFILE(_a_)  \
        STRINGIFY(_a_.h)
/**
 *  This Macro will convert the refered expression into a string and append ".h"
 *  to use it as a include-filename with a path.
 */
#define CREATEINCLUDEFILEWPATH(_p_, _a_)        \
        CREATEINCLUDEFILE(CONCAT1(_p_)\_a_)

/**
 *  This Macro will shift the value a by b Bit-Positions (is equal to the term
 *  a * 2^b).
 */
#define SHIFT(_a_, _b_) \
        (_a_ << _b_)
/**
 *  This Macro will create value with the only Bit set (1) at Bit-Position a.
 *  All other Bits are 0.
 */
#define BV(_a_)         \
        (SHIFT(1, _a_))

/**
 *  This Macro will deal with the compilerwarning "unused argument".
 */
#define UNUSED_ARG(_a_)         \
        (void)_a_

/**
 *  This Macro will create an error if the expression _e_ will be evaluated to
 *  0 at compile-time.
 */
#define BUILD_BUG_ON_ZERO(_e_)    \
        (sizeof(struct { int:-!!(_e_); }))

/**
 *  This Macro will check if the argument is of type array.
 */
#define __must_be_array(_a_)      \
        BUILD_BUG_ON_ZERO(__same_type((_a_), &(_a_)[0]))

/**
 *  This Macro will calculate the number of elements in an array.
 */
#define ARRAY_SIZE(_a_)         \
        (sizeof(_a_) / sizeof(_a_[0]))
//        (sizeof(_a_) / sizeof(_a_[0]) + __must_be_array(_a_))

#ifdef __GNUC__
        #include "gccUtils.h"
#endif /* __GNUC__ */

#endif /* _MYUTILS_H_ */
