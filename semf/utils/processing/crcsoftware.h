/**
 * @file crcsoftware.h
 * @date 24.04.2019
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_UTILS_PROCESSING_CRCSOFTWARE_H_
#define SEMF_UTILS_PROCESSING_CRCSOFTWARE_H_

#include <semf/app/processing/crc.h>
#include <semf/utils/core/debug.h>
#include <algorithm>
#include <cstring>
#include <cstdint>

namespace semf
{
/**
 * @brief Template class for CRC generation in software.
 * @param T CRC data type
 * @param DEFAULT_POLYNOMIAL CRC default polynomial.
 * @param DEFAULT_INITVALUE CRC default init value.
 * @param DEFAULT_FINALXOR CRC default final xor.
 */
template <typename T, T DEFAULT_POLYNOMIAL, T DEFAULT_INITVALUE, T DEFAULT_FINALXOR>
class CrcSoftware : public app::Crc
{
public:
	/**
	 * @brief Constructor.
	 * @param polynomial Uses this polynomial instead of the default polynomial.
	 * @param initValue Uses this initValue instead of the default initValue.
	 * @param finalXor Uses this final xor value instead of the default final xor value.
	 */
	explicit CrcSoftware(T polynomial = DEFAULT_POLYNOMIAL, T initValue = DEFAULT_INITVALUE, T finalXor = DEFAULT_FINALXOR);
	virtual ~CrcSoftware() = default;

	void reset();
	const uint8_t* accumulate(const uint8_t data[], size_t dataSize) override;
	const uint8_t* calculate(const uint8_t data[], size_t dataSize) override;
	bool isEqual(const uint8_t data[]) override;
	size_t bitSize() const override;
	size_t byteSize() const override;

private:
	/**
	 * @brief Inverts the bit sequence of the CRC polynomial.
	 * @param value The value at which the bit sequence is to be invert.
	 * @return The value with the inverted bit sequence.
	 */
	static T invertBitSequence(T value);
	/** Polynomial.*/
	const T m_invertedPolynomial;
	/** Initialization value.*/
	const T m_initValue;
	/** Final xor value.*/
	const T m_finalXor;
	/** CRC value for accumulate.*/
	T m_crc;
};

template <typename T, T DEFAULT_POLYNOMIAL, T DEFAULT_INITVALUE, T DEFAULT_FINALXOR>
CrcSoftware<T, DEFAULT_POLYNOMIAL, DEFAULT_INITVALUE, DEFAULT_FINALXOR>::CrcSoftware(T polynomial, T initValue, T finalXor)
: m_invertedPolynomial(invertBitSequence(polynomial)),
  m_initValue(initValue),
  m_finalXor(finalXor),
  m_crc(initValue ^ m_finalXor)
{
}

template <typename T, T DEFAULT_POLYNOMIAL, T DEFAULT_INITVALUE, T DEFAULT_FINALXOR>
void CrcSoftware<T, DEFAULT_POLYNOMIAL, DEFAULT_INITVALUE, DEFAULT_FINALXOR>::reset()
{
	SEMF_INFO("reset");
	m_crc = m_initValue ^ m_finalXor;
}

template <typename T, T DEFAULT_POLYNOMIAL, T DEFAULT_INITVALUE, T DEFAULT_FINALXOR>
const uint8_t* CrcSoftware<T, DEFAULT_POLYNOMIAL, DEFAULT_INITVALUE, DEFAULT_FINALXOR>::accumulate(const uint8_t data[], size_t dataSize)
{
	m_crc ^= m_finalXor;
	for (size_t i = 0; i < dataSize; i++)
	{
		m_crc ^= data[i];

		for (int i = 0; i < 8; i++)
		{
			T sum = ~((m_crc & 1) - 1);
			m_crc = (m_crc >> 1) ^ (m_invertedPolynomial & sum);
		}
	}
	m_crc ^= m_finalXor;
	SEMF_INFO("accumulated with data %p and size %u to crc value %u", data, dataSize, m_crc);
	return reinterpret_cast<const uint8_t*>(&m_crc);
}

template <typename T, T DEFAULT_POLYNOMIAL, T DEFAULT_INITVALUE, T DEFAULT_FINALXOR>
const uint8_t* CrcSoftware<T, DEFAULT_POLYNOMIAL, DEFAULT_INITVALUE, DEFAULT_FINALXOR>::calculate(const uint8_t data[], size_t dataSize)
{
	m_crc = m_initValue ^ m_finalXor;
	accumulate(data, dataSize);
	SEMF_INFO("calculated with data %p and size %u to crc value %u", data, dataSize, m_crc);
	return reinterpret_cast<const uint8_t*>(&m_crc);
}

template <typename T, T DEFAULT_POLYNOMIAL, T DEFAULT_INITVALUE, T DEFAULT_FINALXOR>
bool CrcSoftware<T, DEFAULT_POLYNOMIAL, DEFAULT_INITVALUE, DEFAULT_FINALXOR>::isEqual(const uint8_t data[])
{
	if (data == nullptr)
	{
		SEMF_ERROR("input is nullptr");
		return false;
	}
	bool e = std::memcmp(reinterpret_cast<const uint8_t*>(&m_crc), data, sizeof(T)) == 0;
	SEMF_INFO("crc equal: %d", e);
	return e;
}

template <typename T, T DEFAULT_POLYNOMIAL, T DEFAULT_INITVALUE, T DEFAULT_FINALXOR>
size_t CrcSoftware<T, DEFAULT_POLYNOMIAL, DEFAULT_INITVALUE, DEFAULT_FINALXOR>::bitSize() const
{
	return sizeof(T) * 8;
}

template <typename T, T DEFAULT_POLYNOMIAL, T DEFAULT_INITVALUE, T DEFAULT_FINALXOR>
size_t CrcSoftware<T, DEFAULT_POLYNOMIAL, DEFAULT_INITVALUE, DEFAULT_FINALXOR>::byteSize() const
{
	size_t ret = bitSize() / 8;
	return (bitSize() % 8 == 0) ? ret : ret + 1;
}

template <typename T, T DEFAULT_POLYNOMIAL, T DEFAULT_INITVALUE, T DEFAULT_FINALXOR>
T CrcSoftware<T, DEFAULT_POLYNOMIAL, DEFAULT_INITVALUE, DEFAULT_FINALXOR>::invertBitSequence(T value)
{
	T retVal = 0;
	for (size_t i = 0; i < sizeof(T) * 8; i++)
	{
		retVal |= (((1 << i) & value) ? 1u : 0u) << (sizeof(T) * 8 - 1 - i);
	}

	return retVal;
}

/** Typedef for Crc32 calculation*/
typedef CrcSoftware<uint32_t, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF> Crc32Software;
/** Typedef for Crc16 calculation*/
typedef CrcSoftware<uint16_t, 0x1021, 0xFFFF, 0xFFFF> Crc16Software;
/** Typedef for Crc8 calculation*/
typedef CrcSoftware<uint8_t, 0x07, 0xFF, 0xFF> Crc8Sofware;
} /* namespace semf */
#endif /* SEMF_UTILS_PROCESSING_CRCSOFTWARE_H_ */
