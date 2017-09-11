/**
 *******************************************************************************
 * @file        ipv4_common.h
 * @version     1.0
 * @date        2016.06.28
 * @author      M. Strosche
 * @brief       Header file for helper-functions for the IPV4-Protocol-Stack.
 *
 * @since       V1.0, 2016.06.28:
 *              -# Initiale Version (MS)
 *
 * @copyright   2017 Michael Strosche.
 *              All rights are reserved.
 *
 * @licence     ...
 *
 *******************************************************************************
 */

#ifndef _IPV4_COMMON_H_
#define _IPV4_COMMON_H_

#include "..\\..\\system.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef union {
	uint32_t raw;
	uint8_t  single[4];
} ipv4_t;

typedef struct {
	uint32_t raw[5];
} ipv4_header_t;

/**------------------------------------------------------------------------------
 *  @fn         IPV4_create
 *  @brief      Calculates the IP-address of type ipv4_t from a 4-element-array.
 *  @param      ip: IP-address as 4-element-array.
 *  @return     IP-address of type ipv4_t.
 *  @pre        None.
 *  @post       None.
 */
inline ipv4_t IPV4_create(uint8_t ip[4])
{
	return *((ipv4_t *)ip);
}

/**------------------------------------------------------------------------------
 *  @fn         IPV4_getSingle
 *  @brief      Returns the single value at the specified index of a given
 *				IP-address.
 *  @param      ip:     Pointer to the IP-address of type ipv4_t.
 *  @param      idx:    Index of the single value [0,3].
 *  @return     Single value at the specified index.
 *  @pre        None.
 *  @post       None.
 */
inline uint8_t IPV4_getSingle(ipv4_t ip, uint8_t idx)
{
	return ip.single[idx];
}

/**------------------------------------------------------------------------------
 *  @fn         IPV4_header_getVersion
 *  @brief      Returns the version-number from the IPV4-header.
 *  @param      ipv4_header:    Pointer to the IPV4-header.
 *  @return     Version-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint8_t IPV4_header_getVersion(ipv4_header_t *ipv4_header)
{
	return (uint8_t)((ipv4_header->raw[0] >> 0) & 0x0000000F);
}

/**------------------------------------------------------------------------------
 *  @fn         IPV4_header_getIHL
 *  @brief      Returns the IHL-number from the IPV4-header.
 *  @param      ipv4_header:    Pointer to the IPV4-header.
 *  @return     IHL-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint8_t IPV4_header_getIHL(ipv4_header_t *ipv4_header)
{
	return (uint8_t)((ipv4_header->raw[0] >> 4) & 0x0000000F);
}

/**------------------------------------------------------------------------------
 *  @fn         IPV4_header_getDSCP
 *  @brief      Returns the DSCP-number from the IPV4-header.
 *  @param      ipv4_header:    Pointer to the IPV4-header.
 *  @return     DSCP-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint8_t IPV4_header_getDSCP(ipv4_header_t *ipv4_header)
{
	return (uint8_t)((ipv4_header->raw[0] >> 8) & 0x0000003F);
}

/**------------------------------------------------------------------------------
 *  @fn         IPV4_header_getECN
 *  @brief      Returns the ECN-number from the IPV4-header.
 *  @param      ipv4_header:    Pointer to the IPV4-header.
 *  @return     ECN-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint8_t IPV4_header_getECN(ipv4_header_t *ipv4_header)
{
	return (uint8_t)((ipv4_header->raw[0] >> 14) & 0x00000003);
}

/**------------------------------------------------------------------------------
 *  @fn         IPV4_header_getTotalLength
 *  @brief      Returns the TotalLength-number from the IPV4-header.
 *  @param      ipv4_header:    Pointer to the IPV4-header.
 *  @return     TotalLength-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint16_t IPV4_header_getTotalLength(ipv4_header_t *ipv4_header)
{
	return (uint16_t)((ipv4_header->raw[0] >> 16) & 0x0000FFFF);
}

/**------------------------------------------------------------------------------
 *  @fn         IPV4_header_getIdentification
 *  @brief      Returns the Identification-number from the IPV4-header.
 *  @param      ipv4_header:    Pointer to the IPV4-header.
 *  @return     Identification-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint16_t IPV4_header_getIdentification(ipv4_header_t *ipv4_header)
{
	return (uint16_t)((ipv4_header->raw[1] >> 0) & 0x0000FFFF);
}

/**------------------------------------------------------------------------------
 *  @fn         IPV4_header_getFlags
 *  @brief      Returns the Flags-number from the IPV4-header.
 *  @param      ipv4_header:    Pointer to the IPV4-header.
 *  @return     Flags-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint8_t IPV4_header_getFlags(ipv4_header_t *ipv4_header)
{
	return (uint8_t)((ipv4_header->raw[1] >> 16) & 0x00000007);
}

/**------------------------------------------------------------------------------
 *  @fn         IPV4_header_getFragmentOffset
 *  @brief      Returns the FragmentOffset-number from the IPV4-header.
 *  @param      ipv4_header:    Pointer to the IPV4-header.
 *  @return     FragmentOffset-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint16_t IPV4_header_getFragmentOffset(ipv4_header_t *ipv4_header)
{
	return (uint16_t)((ipv4_header->raw[1] >> 19) & 0x00001FFF);
}

/**------------------------------------------------------------------------------
 *  @fn         IPV4_header_getTTL
 *  @brief      Returns the TTL-number from the IPV4-header.
 *  @param      ipv4_header:    Pointer to the IPV4-header.
 *  @return     TTL-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint8_t IPV4_header_getTTL(ipv4_header_t *ipv4_header)
{
	return (uint8_t)((ipv4_header->raw[2] >> 0) & 0x000000FF);
}

/**------------------------------------------------------------------------------
 *  @fn         IPV4_header_getProtocol
 *  @brief      Returns the Protocol-number from the IPV4-header.
 *  @param      ipv4_header:    Pointer to the IPV4-header.
 *  @return     Protocol-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint8_t IPV4_header_getProtocol(ipv4_header_t *ipv4_header)
{
	return (uint8_t)((ipv4_header->raw[2] >> 8) & 0x000000FF);
}

/**------------------------------------------------------------------------------
 *  @fn         IPV4_header_getHeaderChecksum
 *  @brief      Returns the HeaderChecksum-number from the IPV4-header.
 *  @param      ipv4_header:    Pointer to the IPV4-header.
 *  @return     HeaderChecksum-number.
 *  @pre        None.
 *  @post       None.
 */
inline uint16_t IPV4_header_getHeaderChecksum(ipv4_header_t *ipv4_header)
{
	return (uint16_t)((ipv4_header->raw[2] >> 16) & 0x0000FFFF);
}

/**------------------------------------------------------------------------------
 *  @fn         IPV4_header_getSourceIPAddress
 *  @brief      Returns the source IP-address from the IPV4-header.
 *  @param      ipv4_header:    Pointer to the IPV4-header.
 *  @return     Source IP-address.
 *  @pre        None.
 *  @post       None.
 */
inline ipv4_t IPV4_header_getSourceIPAddress(ipv4_header_t *ipv4_header)
{
	return *((ipv4_t*)&ipv4_header->raw[3]);
}

/**------------------------------------------------------------------------------
 *  @fn         IPV4_header_getDestinationIPAddress
 *  @brief      Returns the destination IP-address from the IPV4-header.
 *  @param      ipv4_header:    Pointer to the IPV4-header.
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
