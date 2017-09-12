/**
 *******************************************************************************
 * @file        ipv4_common.h
 * @version     0.0.2
 * @date        2017.09.12
 * @author      M. Strosche
 * @brief       Header file for helper-functions for the IPV4-Protocol-Stack.
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

#ifndef _IPV4_COMMON_H_
#define _IPV4_COMMON_H_

#include "..\\..\\system.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  This union can be used to convert between a 32-Bit-Value and its Bytes.
 */
typedef union {
        /**
         *  Raw 32-Bit-Value.
         */
        uint32_t raw;
        
        /**
         *  Array with the Data-Bytes.
         */
        uint8_t  single[4];
} ipv4_t;

/**
 *  This structure reserves space for an IP-Header.
 */
typedef struct {
        /**
         *  Array with the Data-Bytes.
         */
        uint32_t raw[5];
} ipv4_header_t;

/**
 *  Calculates the IP-address of type ipv4_t from a 4-element-array.
 *  @param      ip: IP-address as 4-element-array.
 *  @return     IP-address of type ipv4_t.
 *  @pre        None.
 *  @post       None.
 */
inline ipv4_t IPV4_create(uint8_t ip[4])
{
        return *((ipv4_t *)ip);
}

/**
 *  Returns the single value at the specified index of a given IP-address.
 *  @param      ip: Pointer to the IP-address of type ipv4_t.
 *  @param      idx: Index of the single value [0,3].
 *  @return     Single value at the specified index.
 *  @pre        None.
 *  @post       None.
 */
inline uint8_t IPV4_getSingle(ipv4_t ip, uint8_t idx)
{
        return ip.single[idx];
}

/**
 *  Returns the version-number from the IPV4-header.
 *  @param      ipv4_header: Pointer to the IPV4-header.
 *  @return     Version-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint8_t IPV4_header_getVersion(ipv4_header_t *ipv4_header)
{
        return (uint8_t)((ipv4_header->raw[0] >> 0) & 0x0000000F);
}

/**
 *  Returns the IHL-number from the IPV4-header.
 *  @param      ipv4_header: Pointer to the IPV4-header.
 *  @return     IHL-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint8_t IPV4_header_getIHL(ipv4_header_t *ipv4_header)
{
        return (uint8_t)((ipv4_header->raw[0] >> 4) & 0x0000000F);
}

/**
 *  Returns the DSCP-number from the IPV4-header.
 *  @param      ipv4_header: Pointer to the IPV4-header.
 *  @return     DSCP-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint8_t IPV4_header_getDSCP(ipv4_header_t *ipv4_header)
{
        return (uint8_t)((ipv4_header->raw[0] >> 8) & 0x0000003F);
}

/**
 *  Returns the ECN-number from the IPV4-header.
 *  @param      ipv4_header: Pointer to the IPV4-header.
 *  @return     ECN-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint8_t IPV4_header_getECN(ipv4_header_t *ipv4_header)
{
        return (uint8_t)((ipv4_header->raw[0] >> 14) & 0x00000003);
}

/**
 *  Returns the TotalLength-number from the IPV4-header.
 *  @param      ipv4_header: Pointer to the IPV4-header.
 *  @return     TotalLength-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint16_t IPV4_header_getTotalLength(ipv4_header_t *ipv4_header)
{
        return (uint16_t)((ipv4_header->raw[0] >> 16) & 0x0000FFFF);
}

/**
 *  Returns the Identification-number from the IPV4-header.
 *  @param      ipv4_header: Pointer to the IPV4-header.
 *  @return     Identification-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint16_t IPV4_header_getIdentification(ipv4_header_t *ipv4_header)
{
        return (uint16_t)((ipv4_header->raw[1] >> 0) & 0x0000FFFF);
}

/**
 *  Returns the Flags-number from the IPV4-header.
 *  @param      ipv4_header: Pointer to the IPV4-header.
 *  @return     Flags-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint8_t IPV4_header_getFlags(ipv4_header_t *ipv4_header)
{
        return (uint8_t)((ipv4_header->raw[1] >> 16) & 0x00000007);
}

/**
 *  Returns the FragmentOffset-number from the IPV4-header.
 *  @param      ipv4_header: Pointer to the IPV4-header.
 *  @return     FragmentOffset-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint16_t IPV4_header_getFragmentOffset(ipv4_header_t *ipv4_header)
{
        return (uint16_t)((ipv4_header->raw[1] >> 19) & 0x00001FFF);
}

/**
 *  Returns the TTL-number from the IPV4-header.
 *  @param      ipv4_header: Pointer to the IPV4-header.
 *  @return     TTL-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint8_t IPV4_header_getTTL(ipv4_header_t *ipv4_header)
{
        return (uint8_t)((ipv4_header->raw[2] >> 0) & 0x000000FF);
}

/**
 *  Returns the Protocol-number from the IPV4-header.
 *  @param      ipv4_header: Pointer to the IPV4-header.
 *  @return     Protocol-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint8_t IPV4_header_getProtocol(ipv4_header_t *ipv4_header)
{
        return (uint8_t)((ipv4_header->raw[2] >> 8) & 0x000000FF);
}

/**
 *  Returns the HeaderChecksum-number from the IPV4-header.
 *  @param      ipv4_header: Pointer to the IPV4-header.
 *  @return     HeaderChecksum-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint16_t IPV4_header_getHeaderChecksum(ipv4_header_t *ipv4_header)
{
        return (uint16_t)((ipv4_header->raw[2] >> 16) & 0x0000FFFF);
}

/**
 *  Returns the source IP-address from the IPV4-header.
 *  @param      ipv4_header: Pointer to the IPV4-header.
 *  @return     Source IP-address.
 *  @pre        None.
 *  @post       None.
 */
inline ipv4_t IPV4_header_getSourceIPAddress(ipv4_header_t *ipv4_header)
{
        return *((ipv4_t*)&ipv4_header->raw[3]);
}

/**
 *  Returns the destination IP-address from the IPV4-header.
 *  @param      ipv4_header: Pointer to the IPV4-header.
 *  @return     Destination IP-address.
 *  @pre        None.
 *  @post       None.
 */
inline ipv4_t IPV4_header_getDestinationIPAddress(ipv4_header_t *ipv4_header)
{
        return *((ipv4_t*)&ipv4_header->raw[4]);
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _IPV4_H_ */
