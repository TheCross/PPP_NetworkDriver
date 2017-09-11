/**
 *******************************************************************************
 * @file        databuffer.h
 * @version     0.0.1
 * @date        2017.09.11
 * @author      Michael Strosche (TheCross)
 * @brief       Header-file to handle chained databuffers.
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

#ifndef _DATABUFFER_H_
#define _DATABUFFER_H_

#include "..\\system.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct databuffer_basic_type {
        // Pointer to the previous segment
        struct databuffer_basic_type *prev;
        // Pointer to the next segment
        struct databuffer_basic_type *next;
        // Total length of the data beginning from this segment
        uint32_t                      tot_length;
        // Length of the data in this segment
        uint16_t                      length;
        // Pointer to the data
        uint8_t                      *data;
} databuffer_basic_t;

void databuffer_insertAtEnd(databuffer_basic_t* chain,
                            databuffer_basic_t* newSegment);
void databuffer_insertAtStart(databuffer_basic_t* chain,
                              databuffer_basic_t* newSegment);

void databuffer_insertBefore(databuffer_basic_t* currentSegment,
                             databuffer_basic_t* newSegment);
void databuffer_insertAfter(databuffer_basic_t* currentSegment,
                            databuffer_basic_t* newSegment);

inline void databuffer_create(databuffer_basic_t* databuffer,
                              uint8_t *data,
                              uint16_t length)
{
        if (databuffer != NULL) {
                databuffer->prev = NULL;
                databuffer->next = NULL;
                databuffer->tot_length = length;
                databuffer->length = length;
                databuffer->data = data;
        }
}

void databuffer_copy_partial(databuffer_basic_t* chainDest,
                             uint16_t offsetDest,
                             databuffer_basic_t* chainSrc,
                             uint16_t offsetSrc,
                             uint16_t length);

inline void databuffer_copy(databuffer_basic_t* chainDest,
                            databuffer_basic_t* chainSrc,
                            uint16_t length)
{
        databuffer_copy_partial(chainDest, 0, chainSrc, 0, length);
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _DATABUFFER_H_ */
