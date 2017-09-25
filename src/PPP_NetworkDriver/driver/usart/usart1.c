/**
 *******************************************************************************
 * @file        usart1.c
 * @version     0.0.3
 * @date        2017.09.25
 * @author      Michael Strosche (TheCross)
 * @brief       Source-file for the internal USART1-periphery.
 *
 * @since       V0.0.3, 2017.09.25:
 *                      -# Tabs to spaces. (MS)
 *                      -# Use of UNUSED_ARG in dummyTxCallback. (MS)
 *                      -# Modification of databit-calculation. (MS)
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

#include "usart1.h"
#include "usart1_cfg.h"

#include "..\\..\\system.h"
#include "..\\gpio\\gpio.h"

#ifdef F_CPU
        //#define USART1_BAUD(_speed_) ((F_CPU+_speed_*8UL)/(_speed_*16UL)-1UL)
        #define USART1_BAUD1(_speed_) ((F_CPU/(_speed_*16UL))-1UL)
        #define USART1_BAUD2(_speed_) ((F_CPU/(_speed_*8UL))-1UL)
#else
        #error F_CPU must be defined to calculate the baudrate
#endif /* F_CPU */

#ifdef __AVR_ATmega2560__
        #define USART1_RXD_PORT D
        #define USART1_RXD      (2)
        #define USART1_TXD_PORT D
        #define USART1_TXD      (3)
#else
        #error "undefined processor"
#endif

#if (USART1_DATABITS < 5) || (USART1_DATABITS > 9)
        #error "Illegal setting of USART1_DATABITS! Sould be [5, 9]."
#endif

#define USART1_DATABITS_REGVAL  \
        (USART1_DATABITS - 5 + ((USART1_DATABITS / 9) * 3))

// private function prototypes
static void dummyRxCallback(uint8_t b);
static void dummyTxCallback(void);

// data
static void (*rxFinishedCallback)(uint8_t b) = dummyRxCallback;
static void (*txFinishedCallback)(void) = dummyTxCallback;

// public functions
void usart1_init(uint32_t baudrate)
{
        // Set gpio-pins
        gpio_setPinAsOutput(USART1_RXD_PORT, USART1_RXD);
        gpio_setPinAsOutput(USART1_TXD_PORT, USART1_TXD);
        
        // Set USART1-Registers
        UCSR1A = 0;
        UCSR1B = 0;
        UCSR1C = 0;
        
        UCSR1C |= (USART1_PARITY << UPM10);
        UCSR1C |= (USART1_STOPBITS << USBS1);
        UCSR1C |= ((USART1_DATABITS_REGVAL & 0x03) << UCSZ10);
        UCSR1B |= (((USART1_DATABITS_REGVAL & 0x04) >> 2) << UCSZ12);
  
#ifdef USART1_USE2X
        UCSR1A |= (1 << U2X1);
        UBRR1 = USART1_BAUD2(baudrate);
#else
        UBRR1 = USART1_BAUD1(baudrate);
#endif /* USART1_USE2X */
        
        // Enable RX and TX
        UCSR1B |=  (1 << RXEN1) | (1 << TXEN1);
        // Enable RX and TX interrupts
        UCSR1B |=  (1 << RXCIE1) | (1 << TXCIE1);
}

void usart1_setRxFinishedCallback(void (*callback)(uint8_t b))
{
        if (callback != NULL)
                rxFinishedCallback = callback;
}

void usart1_setTxFinishedCallback(void (*callback)(void))
{
        if (callback != NULL)
                txFinishedCallback = callback;
}

// private functions
static void dummyRxCallback(uint8_t b)
{
        UNUSED_ARG(b);
}

static void dummyTxCallback(void)
{
        // ...
}


// interrupt service routines
ISR(USART1_RX_vect)
{
        cli();
        rxFinishedCallback(UDR1);
        sei();
}

ISR(USART1_TX_vect)
{
        cli();
        txFinishedCallback();
        sei();
}