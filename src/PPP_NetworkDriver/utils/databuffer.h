/**
 *******************************************************************************
 * @file        databuffer.h
 * @version     0.0.3
 * @date        2017.09.25
 * @author      Michael Strosche (TheCross)
 * @brief       Header-file to handle chained databuffers.
 *
 * @since       V0.0.3, 2017.09.25:
 *                      -# Changed from inline to macro. (MS)
 *                      -# No typedefs for struct and enum. (MS)
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

#ifndef _DATABUFFER_H_
#define _DATABUFFER_H_

#include "..\\system.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  This structure defines a basic DataBuffer-Element.
 */
struct databuffer_basic_t {
        /**
         * Pointer to the previous segment.
         */
        struct databuffer_basic_t      *prev;
        
        /**
         * Pointer to the next segment.
         */
        struct databuffer_basic_t      *next;
        
        /**
         * Total length of the data beginning from this segment.
         */
        uint32_t                        tot_length;
        
        /**
         * Length of the data in this segment.
         */
        uint16_t                        length;
        
        /**
         * Pointer to the Data-Array.
         */
        uint8_t                        *data;
};

/**
 *  Inserts a DataBuffer-Segment (it can also be a chain) at the end of another
 *  DataBuffer-Chain.
 *  @param      chain: DataBuffer-Chain.
 *  @param      newSegment: DataBuffer-Element(s) to insert at the end.
 *  @return     None.
 *  @pre        None.
 *  @post       The DataBuffer-Element(s) had been inserted at the end of the
 *              DataBuffer-Chain and the total length of the whole
 *              DataBuffer-Chain had been updated.
 */
void databuffer_insertAtEnd(struct databuffer_basic_t* chain,
                            struct databuffer_basic_t* newSegment);

/**
 *  Inserts a DataBuffer-Segment (it can also be a chain) at the start of
 *  another DataBuffer-Chain.
 *  @param      chain: DataBuffer-Chain.
 *  @param      newSegment: DataBuffer-Element(s) to insert at the start.
 *  @return     None.
 *  @pre        None.
 *  @post       The DataBuffer-Element(s) had been inserted at the start of the
 *              DataBuffer-Chain and the total length of the whole
 *              DataBuffer-Chain had been updated.
 */
void databuffer_insertAtStart(struct databuffer_basic_t* chain,
                              struct databuffer_basic_t* newSegment);

/**
 *  Inserts a DataBuffer-Segment (it can also be a chain) before another
 *  specified DataBuffer-Element. This Element can also be at any position in a
 *  DataBuffer-Chain.
 *  @param      currentSegment: DataBuffer-Element.
 *  @param      newSegment: DataBuffer-Element(s) to insert at before the
 *              specified DataBuffer-Element.
 *  @return     None.
 *  @pre        None.
 *  @post       The DataBuffer-Element(s) had been inserted before the specified
 *              DataBuffer-Element and the total length of the whole
 *              DataBuffer-Chain had been updated.
 */
void databuffer_insertBefore(struct databuffer_basic_t* currentSegment,
                             struct databuffer_basic_t* newSegment);

/**
 *  Inserts a DataBuffer-Segment (it can also be a chain) after another
 *  specified DataBuffer-Element. This Element can also be at any position in a
 *  DataBuffer-Chain.
 *  @param      currentSegment: DataBuffer-Element.
 *  @param      newSegment: DataBuffer-Element(s) to insert at after the
 *              specified DataBuffer-Element.
 *  @return     None.
 *  @pre        None.
 *  @post       The DataBuffer-Element(s) had been inserted after the specified
 *              DataBuffer-Element and the total length of the whole
 *              DataBuffer-Chain had been updated.
 */
void databuffer_insertAfter(struct databuffer_basic_t* currentSegment,
                            struct databuffer_basic_t* newSegment);

/**
 *  Creates a new single DataBuffer-Element by linking the corresponding
 *  Data-Array and initializing the other elements in the structure.
 *  @param      databuffer: DataBuffer-Element.
 *  @param      data: Pointer to a Data-Array holding the data of the
 *              DataBuffer-Element.
 *  @param      length: Maximum Number of Bytes the Data-Array can hold.
 *  @return     None.
 *  @pre        None.
 *  @post       The DataBuffer-Element has been created.
 */
inline void databuffer_create(struct databuffer_basic_t *databuffer,
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

/**
 *  Copies a specific amount of data from one DataBuffer-Chain to another
 *  beginning from the specified offsets in each DataBuffer-Chain.
 *  @param      chainDest: First DataBuffer-Element of a chain that should hold
 *              the copied data.
 *  @param      offsetDest: Offset in the Destination-Chain in Bytes.
 *  @param      chainSrc: First DataBuffer-Element of a chain that holds the
 *              data that has to be copied.
 *  @param      offsetSrc: Offset in the Source-Chain in Bytes.
 *  @param      length: Number of Bytes to copy.
 *  @return     None.
 *  @pre        None.
 *  @post       The data has been copied from the source to the destination.
 */
void databuffer_copy_partial(struct databuffer_basic_t* chainDest,
                             uint16_t offsetDest,
                             struct databuffer_basic_t* chainSrc,
                             uint16_t offsetSrc,
                             uint16_t length);

/**
 *  Copies a specific amount of data from one DataBuffer-Chain to another
 *  beginning from the first Byte in each DataBuffer-Chain.
 *  @param      _chainDest_: First DataBuffer-Element of a chain that should
 *              hold the copied data.
 *  @param      _chainSrc_: First DataBuffer-Element of a chain that holds the
 *              data that has to be copied.
 *  @param      _length_: Number of Bytes to copy.
 *  @return     None.
 *  @pre        None.
 *  @post       The data has been copied from the source to the destination.
 *  @see        databuffer_copy_partial
 */
#define databuffer_copy(_chainDest_, _chainSrc_, _length_)      \
        databuffer_copy_partial(_chainDest_, 0, _chainSrc_, 0, _length_)

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _DATABUFFER_H_ */
