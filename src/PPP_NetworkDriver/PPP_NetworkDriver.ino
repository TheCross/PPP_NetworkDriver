/**
 *******************************************************************************
 * @file        PPP_NetworkDriver.ino
 * @version     0.0.2
 * @date        2017.09.12
 * @author      Michael Strosche (TheCross)
 * @brief       Main source-file.
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

#include "system.h"

#include <util/delay.h>

#include "utils\\serialConsole.h"
#include "driver\\net\\PPP.h"
#include "driver\\net\\LCP.h"
#include "driver\\net\\IPV4.h"
#include "driver\\net\\UDP.h"
#include "driver\\net\\TCP.h"

// This is only for offline-testing
uint8_t dataFirstPacket[] = {0x01, 0x01, 0x00, 0x14, 0x02, 0x06, 0x00, 0x00,
                             0x00, 0x00, 0x05, 0x06, 0x50, 0x36, 0xE9, 0xEB,
                             0x07, 0x02, 0x08, 0x02
                            };
databuffer_basic_t dbFirstPacket;

void setup() {
          serialConsole_init();

          net_PPP_init();
          net_LCP_init();
          //net_IPV4_init();
          //net_UDP_init();
          //net_TCP_init();

          _delay_ms(100);

          // This is only for offline-testing
          //databuffer_create(&dbFirstPacket, dataFirstPacket, sizeof(dataFirstPacket) / sizeof(uint8_t));
          //net_PPP_txDataBuffer(NETPPP_LCP, &dbFirstPacket);

          _delay_ms(100);
}

void loop() {
          // check for received packets and process them
          net_PPP_loop();
}
