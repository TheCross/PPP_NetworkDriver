/**
 *******************************************************************************
 * @file        serialConsole.h
 * @version     0.0.2
 * @date        2017.09.12
 * @author      Michael Strosche (TheCross)
 * @brief       Source-file to handle input/output of serial data (strings,
 *              etc.).
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

#include "serialConsole.h"
#include "serialConsole_cfg.h"

#include "..\\system.h"

#define SERIALCONSOLE_UARTPREFIX \
        CONCAT2(SERIALCONSOLE_UARTTYPE, SERIALCONSOLE_UARTNUMBER)
#define SERIALCONSOLE_UARTINCLUDE  \
        CREATEINCLUDEFILEWPATH(SERIALCONSOLE_UARTPATH, SERIALCONSOLE_UARTPREFIX)
#define serialConsole_uart_init \
        CONCAT2(SERIALCONSOLE_UARTPREFIX, _init)
#define serialConsole_uart_setRxFinishedCallback \
        CONCAT2(SERIALCONSOLE_UARTPREFIX, _setRxFinishedCallback)
#define serialConsole_uart_setTxFinishedCallback \
        CONCAT2(SERIALCONSOLE_UARTPREFIX, _setTxFinishedCallback)
#define serialConsole_uart_txByte \
        CONCAT2(SERIALCONSOLE_UARTPREFIX, _txByte)

#include SERIALCONSOLE_UARTINCLUDE


// private function prototypes
static void rxCallback(uint8_t b);
static void txCallback(void);


// data
static volatile uint8_t  rxBuffer[SERIALCONSOLE_RX_BUFFERSIZE];
static volatile uint16_t rxBufferReadIndex;
static volatile uint16_t rxBufferSize;
static volatile uint8_t  txBuffer[SERIALCONSOLE_TX_BUFFERSIZE];
static volatile uint16_t txBufferReadIndex;
static volatile uint16_t txBufferSize;
static volatile bool     isTransmitting;


// public functions
void serialConsole_init(void)
{
        serialConsole_uart_init(SERIALCONSOLE_BAUDRATE);
        serialConsole_uart_setRxFinishedCallback(rxCallback);
        serialConsole_uart_setTxFinishedCallback(txCallback);
        
        rxBufferReadIndex = 0;
        rxBufferSize      = 0;
        txBufferReadIndex = 0;
        txBufferSize      = 0;
        isTransmitting = false;
}

uint16_t serialConsole_getRxByteCount(void)
{
        return rxBufferSize;
}

uint8_t serialConsole_getRxByte(uint8_t *b)
{
        // check if bytes are available
        if (rxBufferSize > 0) {
                cli();
                *b = rxBuffer[rxBufferReadIndex];
                
                // adjust rxReadIndex
                rxBufferReadIndex = (rxBufferReadIndex + 1)
                                    % SERIALCONSOLE_RX_BUFFERSIZE;
                rxBufferSize--;
                sei();
                
                return 1;
        }
        
        return 0;
}

uint16_t serialConsole_getRxBytes(uint8_t *b, uint16_t size)
{
        uint16_t numberOfReadBytes = 0;
        
        // read bytes one by one from the buffer
        while ((numberOfReadBytes < size) &&
               (serialConsole_getRxByte(b++) != 0))
                numberOfReadBytes++;
        
        return numberOfReadBytes;
}

void serialConsole_txByte(uint8_t b)
{
        if (isTransmitting) {
                // save in buffer
                if (txBufferSize < SERIALCONSOLE_TX_BUFFERSIZE) {
                        cli();
                        txBuffer[(txBufferReadIndex + txBufferSize)
                                 % SERIALCONSOLE_TX_BUFFERSIZE] = b;

                        // adjust txBufferSize
                        txBufferSize++;
                        sei();
                }
        } else {
                // direct transmit
                isTransmitting = true;
                serialConsole_uart_txByte(b);
        }
}

void serialConsole_txBytes(uint8_t *b, uint16_t length)
{
        // transmits bytes one-by-one
        while (length > 0) {
                // wait until the buffer has space for the next byte
                while (txBufferSize >= SERIALCONSOLE_TX_BUFFERSIZE);
                
                serialConsole_txByte(*b);
                b++;
                length--;
        }
}


// private functions
static void rxCallback(uint8_t b)
{
        if (rxBufferSize < SERIALCONSOLE_RX_BUFFERSIZE) {
                rxBuffer[(rxBufferReadIndex + rxBufferSize)
                         % SERIALCONSOLE_RX_BUFFERSIZE] = b;
                
                // adjust rxBufferSize
                rxBufferSize++;
        }
}

static void txCallback(void)
{
        // this function transmits one byte at a time
        if (txBufferSize > 0) {
                serialConsole_uart_txByte(txBuffer[txBufferReadIndex]);
                
                // adjust txReadIndex
                txBufferReadIndex = (txBufferReadIndex + 1)
                                    % SERIALCONSOLE_TX_BUFFERSIZE;
                txBufferSize--;
        } else {
                isTransmitting = false;
        }
}


// interrupt service routines
