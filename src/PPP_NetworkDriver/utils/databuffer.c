/**
 *******************************************************************************
 * @file        databuffer.c
 * @version     0.0.2
 * @date        2017.09.12
 * @author      Michael Strosche (TheCross)
 * @brief       Sorce-file to handle chained databuffers.
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

#include "databuffer.h"

#include "..\\system.h"

void databuffer_adjustLength(databuffer_basic_t* chain);

void databuffer_insertAtEnd(databuffer_basic_t* chain,
                            databuffer_basic_t* newSegment)
{
        databuffer_basic_t *chainEnd = chain;
        
        while (chainEnd->next != NULL)
                chainEnd = chainEnd->next;
        
        chainEnd->next = newSegment;
        newSegment->prev = chainEnd;
        
        databuffer_adjustLength(chain);
}

void databuffer_insertAtStart(databuffer_basic_t* chain,
                              databuffer_basic_t* newSegment)
{
        databuffer_basic_t *chainHead = chain;
        databuffer_basic_t *newSegmentEnd = newSegment;
        
        while (chainHead->prev != NULL)
                chainHead = chainHead->prev;
        while (newSegmentEnd->next != NULL)
                newSegmentEnd = newSegmentEnd->next;
        
        newSegmentEnd->next = chainHead;
        chainHead->prev = newSegmentEnd;
        
        databuffer_adjustLength(chain);
}

void databuffer_insertBefore(databuffer_basic_t* currentSegment,
                             databuffer_basic_t* newSegment)
{
        if (currentSegment->prev == NULL) {
                databuffer_insertAtStart(currentSegment, newSegment);
        } else {
                databuffer_insertAfter(currentSegment->prev, newSegment);
        }
}

void databuffer_insertAfter(databuffer_basic_t* currentSegment,
                            databuffer_basic_t* newSegment)
{
        if (currentSegment->next == NULL) {
                databuffer_insertAtEnd(currentSegment, newSegment);
        } else {
                databuffer_basic_t *newSegmentEnd = newSegment;
                
                while (newSegmentEnd->next != NULL)
                        newSegmentEnd = newSegmentEnd->next;
                
                newSegmentEnd->next = currentSegment->next;
                newSegment->prev = currentSegment;
                currentSegment->next->prev = newSegmentEnd;
                currentSegment->next = newSegment;
        
                databuffer_adjustLength(currentSegment);
        }
}

void databuffer_adjustLength(databuffer_basic_t* chain)
{
        // go to end of the list
        databuffer_basic_t *chainEnd = chain;
        while (chainEnd->next != NULL)
                chainEnd = chainEnd->next;
        
        // move to beginning of the list and accumulate the length
        databuffer_basic_t *chainPosition = chainEnd;
        while (chainPosition != NULL) {
                if (chainPosition->next != NULL)
                        chainPosition->tot_length =
                                chainPosition->next->tot_length +
                                chainPosition->length;
                else
                        chainPosition->tot_length = chainPosition->length;
                
                chainPosition = chainPosition->prev;
        }
}

void databuffer_copy_partial(databuffer_basic_t* chainDest,
                             uint16_t offsetDest,
                             databuffer_basic_t* chainSrc,
                             uint16_t offsetSrc,
                             uint16_t length)
{
        if ((chainDest == NULL) || (chainSrc == NULL))
                return;
                
        if ((offsetDest >= chainDest->tot_length) ||
            (offsetSrc >= chainSrc->tot_length))
                return;

        // limit amount of bytes to copy
        length = min(chainDest->tot_length - offsetDest, length);
        length = min(chainSrc->tot_length - offsetSrc, length);

        // jump to read-position in destination and source
        while ((chainDest != NULL) && (offsetDest >= chainDest->length)) {
                offsetDest -= chainDest->length;
                chainDest = chainDest->next;
        }
        while ((chainSrc != NULL) && (offsetSrc >= chainSrc->length)) {
                offsetSrc -= chainSrc->length;
                chainSrc = chainSrc->next;
        }

        // copy
        while ((chainDest != NULL) && (chainSrc != NULL) && (length--)) {
                chainDest->data[offsetDest++] = chainSrc->data[offsetSrc++];
                if (offsetDest >= chainDest->length) {
                        offsetDest -= chainDest->length;
                        chainDest = chainDest->next;
                }
                if (offsetSrc >= chainSrc->length) {
                        offsetSrc -= chainSrc->length;
                        chainSrc = chainSrc->next;
                }
        }
}

