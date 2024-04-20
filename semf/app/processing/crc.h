/**
 * @file crc.h
 * @date 07.06.2023
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 * 			  For detailed information, read the license file in the project
 * 			  root directory.
 */

#ifndef SEMF_APP_PROCESSING_CRC_H_
#define SEMF_APP_PROCESSING_CRC_H_

#include <cstddef>
#include <cstdint>

namespace semf
{
namespace app
{
/**
 * @brief Interface for implementing cyclic redundancy check (CRC).
 */
class Crc
{
public:
	virtual ~Crc() = default;

	/**
	 * @brief Resets internal values to their initial values.
	 * @see accumulate().
	 */
	virtual void reset() = 0;
	/**
	 * @brief Computes the CRC of the input data using a combination of
	 * the previous CRC value and the new one.
	 * @note This function is helpful if the data is not completely stored in
	 * the memory and is called up piece by piece from another memory.
	 * @param data Data over which a CRC value will be computed.
	 * @param size Size of the input data.
	 * @return Pointer to the calculated CRC value.
	 * @see reset().
	 */
	virtual const uint8_t* accumulate(const uint8_t data[], size_t size) = 0;
	/**
	 * @brief Calculates the CRC of the input \c data.
	 * @note This function is helpful if the data is completely stored in
	 * the memory, unlike in \c accumulate() function.
	 * @param data Data over which a CRC value will be computed.
	 * @param size Size of the input data.
	 * @return Pointer to the calculated CRC value.
	 */
	virtual const uint8_t* calculate(const uint8_t data[], size_t size) = 0;
	/**
	 * @brief Checks if previously calculated CRC is equal to the input value.
	 * @param data CRC value to compare with calculated CRC value.
	 * @return \c true for CRCs are equal, otherwise \c false.
	 */
	virtual bool isEqual(const uint8_t data[]) = 0;
	/**
	 * @brief Returns the size of the CRC in bits.
	 * @return Number of bits of the CRC value.
	 */
	virtual size_t bitSize() const = 0;
	/**
	 * @brief Returns the size of the CRC in bytes.
	 * @return Number of bytes of the CRC value.
	 */
	virtual size_t byteSize() const = 0;
};
} /* namespace app */
} /* namespace semf */
#endif /* SEMF_APP_PROCESSING_CRC_H_ */
