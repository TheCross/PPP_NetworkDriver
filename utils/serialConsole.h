/**
 *******************************************************************************
 * @file        serialConsole.h
 * @version     0.0.1
 * @date        2017.09.11
 * @author      Michael Strosche (TheCross)
 * @brief       Header-file to handle input/output of serial data (strings,
                etc.).
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

#ifndef _SERIALCONSOLE_H_
#define _SERIALCONSOLE_H_

#include "..\\system.h"

#include "databuffer.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

void serialConsole_init(void);

uint16_t serialConsole_getRxByteCount(void);
uint8_t  serialConsole_getRxByte(uint8_t *b);
uint16_t serialConsole_getRxBytes(uint8_t *b, uint16_t size);

void serialConsole_txByte(uint8_t b);
void serialConsole_txBytes(uint8_t *b, uint16_t length);

static inline void serialConsole_txString(char *s)
{
        serialConsole_txBytes((uint8_t *)s, strlen(s));
}

static inline void serialConsole_txDatabuffer(databuffer_basic_t *chain)
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
