/**
 * @file bitsetutility.h
 * @date 07.01.2022
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_UTILS_PROCESSING_BITSETUTILITY_H_
#define SEMF_UTILS_PROCESSING_BITSETUTILITY_H_

#include <cstdint>
#include <bitset>

namespace semf
{
/**
 * @brief Useful helper functions for handeling bitsets.
 * @note They are used in ecc classes.
 */
class BitsetUtility
{
public:
	/**
	 * @brief Returns the parity of bits in a bitset.
	 * @tparam Number of bits in the bit set.
	 * @param bits The bits for which parity is to be calculated.
	 * @return true if ther parity is odd.
	 */
	template <size_t N>
	static bool parityIsOdd(std::bitset<N> bits);

	/**
	 * @brief Copies a part of an array into a bitset.
	 * @tparam Number of bits in the bit set.
	 * @param data The array with the data.
	 * @param dataSize The size of the data.
	 * @param firstBit The first bit to be copied into the bitset.
	 * @return The bitset with the bits.
	 */
	template <size_t N>
	static std::bitset<N> createBitset(const uint8_t data[], size_t dataSize, size_t firstBit);

	/**
	 * @brief Insert a bitset in a array.
	 * @tparam N Number of bits in the bit set.
	 * @param bitset The bitset which should be inserted into the array.
	 * @param buffer The array in which the bitset is stored.
	 * @param bufferSize The size of the buffer.
	 * @param firstBit The position in which the bitset is stored in the array.
	 * @return The number of bits written into the array.
	 */
	template <size_t N>
	static size_t insertBitsetInBuffer(std::bitset<N> bitset, uint8_t buffer[], size_t bufferSize, size_t firstBit);
};

template <size_t N>
bool BitsetUtility::parityIsOdd(std::bitset<N> bits)
{
	return bits.count() % 2;
}

template <size_t N>
std::bitset<N> BitsetUtility::createBitset(const uint8_t data[], size_t dataSize, size_t firstBit)
{
	std::bitset<N> bitset;

	for (size_t i = 0; (i < N) && (((firstBit + i) / 8) < dataSize); i++)
	{
		bitset.set(i, data[(firstBit + i) / 8] & (1 << ((firstBit + i) % 8)));
	}

	return bitset;
}

template <size_t N>
static size_t insertBitsetInBuffer(std::bitset<N> bitset, uint8_t buffer[], size_t bufferSize, size_t firstBit)
{
	size_t i = 0;

	for (; (i < N) && (((firstBit + i) / 8) < bufferSize); i++)
	{
		if (bitset[i])
			buffer[(firstBit + i) / 8] |= static_cast<uint8_t>((1 << ((firstBit + i) % 8)));

		else
			buffer[(firstBit + i) / 8] &= static_cast<uint8_t>(~(1 << ((firstBit + i) % 8)));
	}

	return i;
}
} /* namespace semf */
#endif  // SEMF_UTILS_PROCESSING_BITSETUTILITY_H_
