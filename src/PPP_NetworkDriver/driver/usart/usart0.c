/**
 *******************************************************************************
 * @file        usart0.c
 * @version     0.0.2
 * @date        2017.09.12
 * @author      Michael Strosche (TheCross)
 * @brief       Source-file for the internal USART0-periphery.
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

#include "usart0.h"
#include "usart0_cfg.h"

#include "..\\..\\system.h"
#include "..\\gpio\\gpio.h"

#ifdef F_CPU
        //#define USART0_BAUD(_speed_) ((F_CPU+_speed_*8UL)/(_speed_*16UL)-1UL)
        #define USART0_BAUD1(_speed_) ((F_CPU/(_speed_*16UL))-1UL)
        #define USART0_BAUD2(_speed_) ((F_CPU/(_speed_*8UL))-1UL)
#else
        #error F_CPU must be defined to calculate the baudrate
#endif /* F_CPU */

#ifdef __AVR_ATmega2560__
        #define USART0_RXD_PORT E
        #define USART0_RXD      (0)
        #define USART0_TXD_PORT E
        #define USART0_TXD      (1)
#else
        #error "undefined processor"
#endif

#if USART0_DATABITS == USART0_DATABITS_9
        #error "9th databit currently not supported"
#endif

// private function prototypes
static void dummyRxCallback(uint8_t b);
static void dummyTxCallback(void);

// data
static void (*rxFinishedCallback)(uint8_t b) = dummyRxCallback;
static void (*txFinishedCallback)(void) = dummyTxCallback;

// public functions
void usart0_init(uint32_t baudrate)
{
        // Set gpio-pins
        gpio_setPinAsOutput(USART0_RXD_PORT, USART0_RXD);
        gpio_setPinAsOutput(USART0_TXD_PORT, USART0_TXD);
        
        // Set USART0-Registers
        UCSR0A = 0;
        UCSR0B = 0;
        UCSR0C = 0;
        
        UCSR0C |= (USART0_PARITY << UPM00);
        UCSR0C |= (USART0_STOPBITS << USBS0);
        UCSR0C |= ((USART0_DATABITS & 0x03) << UCSZ00);
        UCSR0B |= (((USART0_DATABITS & 0x04) >> 2) << UCSZ02);
  
#ifdef USART0_USE2X
        UCSR0A |= (1 << U2X0);
        UBRR0 = USART0_BAUD2(baudrate);
#else
        UBRR0 = USART0_BAUD1(baudrate);
#endif /* USART0_USE2X */
        
        // Enable RX and TX
        UCSR0B |=  (1 << RXEN0) | (1 << TXEN0);
        // Enable RX and TX interrupts
    UCSR0B |=  (1 << RXCIE0) | (1 << TXCIE0);
}

void usart0_setRxFinishedCallback(void (*callback)(uint8_t b))
{
        if (callback != NULL)
                rxFinishedCallback = callback;
}

void usart0_setTxFinishedCallback(void (*callback)(void))
{
        if (callback != NULL)
                txFinishedCallback = callback;
}

// private functions
static void dummyRxCallback(uint8_t b)
{
  // This line is to remove the warning about an unused parameter 'b'
  b = b;
}

static void dummyTxCallback(void) {}


// interrupt service routines
ISR(USART0_RX_vect)
{
        cli();
        rxFinishedCallback(UDR0);
        sei();
}

ISR(USART0_TX_vect)
{
        cli();
        txFinishedCallback();
        sei();
}
