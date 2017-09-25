/**
 *******************************************************************************
 * @file        serialConsole.h
 * @version     0.0.3
 * @date        2017.09.25
 * @author      Michael Strosche (TheCross)
 * @brief       Header-file to handle input/output of serial data (strings,
                etc.).
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

#ifndef _SERIALCONSOLE_H_
#define _SERIALCONSOLE_H_

#include "..\\system.h"

#include "databuffer.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Initializes the serial console.
 *  @return     None.
 *  @pre        None.
 *  @post       This module and the corresponding U(S)ART are initialized.
 */
void serialConsole_init(void);

/**
 *  Returns the number of unread received Bytes in the RX-Buffer.
 *  @return     Number of unread received Bytes in the RX-Buffer.
 *  @pre        The function serialConsole_init had been called.
 *  @post       None.
 */
uint16_t serialConsole_getRxByteCount(void);

/**
 *  Returns the first unread Byte from the RX-Buffer.
 *  @param      b: Pointer to a buffer that can hold the read Byte.
 *  @return     Number of read Bytes.
 *  @pre        The function serialConsole_init had been called.
 *  @post       If there had been a unread Byte in the RX-Buffer it had been
 *              copied into the Buffer specified by the parameter. The number of
 *              Bytes in the RX-Buffer had been decreased.
 */
uint8_t  serialConsole_getRxByte(uint8_t *b);

/**
 *  Returns the first x unread Bytes from the RX-Buffer.
 *  @param      b: Pointer to a buffer that can hold the read Bytes.
 *  @param      size: Number of Bytes to read.
 *  @return     Number of read Bytes.
 *  @pre        The function serialConsole_init had been called.
 *  @post       If there had been unread Byte in the RX-Buffer they had been
 *              copied into the Buffer specified by the parameter. The number of
 *              Bytes in the RX-Buffer had been decreased by the amount of read
 *              Bytes.
 */
uint16_t serialConsole_getRxBytes(uint8_t *b, uint16_t size);

/**
 *  Transmits a Byte. If there is a transmission in progress the Byte will be
 *  put at the end of the TX-Buffer. If the TX-Buffer is full it will wait until
 *  there is enough space to append the Byte.
 *  @param      b: Byte to transmit.
 *  @return     None.
 *  @pre        The function serialConsole_init had been called.
 *  @post       The Byte had ben put into the TX-Buffer (hardware or software).
 */
void serialConsole_txByte(uint8_t b);

/**
 *  Transmits a number of Bytes. It will transmit them Byte-after-Byte.       @n
 *  If there is a transmission in progress the next Byte will be put at the end
 *  of the TX-Buffer. If the TX-Buffer is full it will wait until there is
 *  enough space to append the Byte.
 *  @param      b: Pointer to an Array holding the Bytes to transmit.
 *  @param      length: Number of Bytes to transmit.
 *  @return     None.
 *  @pre        The function serialConsole_init had been called.
 *  @post       The Bytes had ben put into the TX-Buffer (hardware or software).
 */
void serialConsole_txBytes(uint8_t *b, uint16_t length);

/**
 *  Transmits a String (zero-terminated Character-Array).                     @n
 *  This function will block until the last character had been put into the
 *  TX-Buffer.
 *  @param      _s_: Pointer to a zero-terminated Character-Array to transmit.
 *  @return     None.
 *  @pre        The function serialConsole_init had been called.
 *  @post       The String had ben put into the TX-Buffer (hardware or
 *              software).
 *  @see        serialConsole_txBytes
 */
#define serialConsole_txString(_s_)     \
        serialConsole_txBytes((uint8_t *)_s_, strlen(_s_))

/**
 *  Transmits a DataBuffer.                                                   @n
 *  This function will block until the last Byte had been put into the
 *  TX-Buffer.
 *  @param      chain: Pointer to the first Element of the DataBuffer-Chain.
 *  @return     None.
 *  @pre        The function serialConsole_init had been called.
 *  @post       The DataBuffer had ben put into the TX-Buffer (hardware or
 *              software).
 *  @see        serialConsole_txBytes
 */
inline void serialConsole_txDatabuffer(struct databuffer_basic_t *chain)
{
        while (chain != NULL) {
                serialConsole_txBytes(chain->data, chain->length);
                chain = chain->next;
        }
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _SERIALCONSOLE_H_ */
