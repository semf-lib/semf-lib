/**
 * @file endian.h
 * 
 * @date 26.07.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory
 */

#ifndef PROCESSING_ENDIAN_ENDIAN_H_
#define PROCESSING_ENDIAN_ENDIAN_H_

#include <Processing/stdworkarounds/bit>
#include <Processing/Endian/bigendianinteger.h>
#include <Processing/Endian/littleendianinteger.h>
#include <cstdint>
#include <algorithm>

namespace semf
{
using int16be = BigEndianInteger<int16_t>;
using int16le = LittleEndianInteger<int16_t>;
using int32be = BigEndianInteger<int32_t>;
using int32le = LittleEndianInteger<int32_t>;
using int64be = BigEndianInteger<int64_t>;
using int64le = LittleEndianInteger<int64_t>;
using uint16be = BigEndianInteger<uint16_t>;
using uint16le = LittleEndianInteger<uint16_t>;
using uint32be = BigEndianInteger<uint32_t>;
using uint32le = LittleEndianInteger<uint32_t>;
using uint64be = BigEndianInteger<uint64_t>;
using uint64le = LittleEndianInteger<uint64_t>;
/**
 * @brief The \c Endian class provides a set of functions for handeling the conversion of data with a specific endianess.
 */
class Endian
{
public:
	/**
	 * @brief Reads \c size big-endian numbers from \c data an stores them in a native representation in \c buffer.
	 * @tparam T Native type of the numbers.
	 * @param data Incomming big-endian numbers.
	 * @param size The count (!) of numbers.
	 * @param buffer Buffer for storing the numbers.
	 */
	template<typename T>
	static void fromBigEndian(const void* data, size_t size, void* buffer);
	/**
	 * @brief Reads a single big-endian number from the given address and returns it converted to a native type.
	 * @tparam T Native type of the number.
	 * @param data Address of a big-endian number.
	 * @return Converted native number.
	 */
	template<typename T>
	static T fromBigEndian(const void* data);
	/**
	 * @brief Converts the given big-endian number to a native number.
	 * @tparam T Native type of the number.
	 * @param data Number to convert.
	 * @return Converted number.
	 */
	template<typename T>
	static T fromBigEndian(T data);
	/**
	 * @brief Reads \c size little-endian numbers from \c data an stores them in a native representation in \c buffer.
	 * @tparam T Native type of the numbers.
	 * @param data Incomming little-endian numbers.
	 * @param size The count (!) of numbers.
	 * @param buffer Buffer for storing the numbers.
	 */
	template<typename T>
	static void fromLittleEndian(const void* data, size_t size, void* buffer);
	/**
	 * @brief Reads a single little-endian number from the given address and returns it converted to a native type.
	 * @tparam T Native type of the number.
	 * @param data Address of a little-endian number.
	 * @return Converted native number.
	 */
	template<typename T>
	static T fromLittleEndian(const void* data);
	/**
	 * @brief Converts the given little-endian number to a native number.
	 * @tparam T Native type of the number.
	 * @param data Number to convert.
	 * @return Converted number.
	 */
	template<typename T>
	static T fromLittleEndian(T data);
	/**
	 * @brief Reads \c size native stored numbers and converts them into big endian numbers.
	 * @tparam T Native type of the number.
	 * @param data Incomming native numbers.
	 * @param size Count (!) of native numbers.
	 * @param buffer Buffer for storing the converted numbers.
	 */
	template<typename T>
	static void toBigEndian(const void* data, size_t size, void* buffer);
	/**
	 * @brief Converts the given number to a big-endian number and stores at the given address.
	 * @tparam T Native type of the number.
	 * @param data Number to convert.
	 * @param buffer Buffer for storing.
	 */
	template<typename T>
	static void toBigEndian(T data, void* buffer);
	/**
	 * @brief Converts the given number to a big-endian number.
	 * @tparam T Native type of the number.
	 * @param data Number to convert. 
	 * @return Converted number.
	 */
	template<typename T>
	static T toBigEndian(T data);
	/**
	 * @brief Reads \c size native stored numbers and converts them into little-endian numbers.
	 * @tparam T Native type of the number.
	 * @param data Incomming native numbers.
	 * @param size Count (!) of native numbers.
	 * @param buffer Buffer for storing the converted numbers.
	 */
	template<typename T>
	static void toLittleEndian(const void* data, size_t size, void* buffer);
	/**
	 * @brief Converts the given number to a little-endian number.
	 * @tparam T Native type of the number.
	 * @param data Number to convert.
	 * @param buffer Buffer for storing.
	 */
	template<typename T>
	static void toLittleEndian(T data, void* buffer);
	/**
	 * @brief Converts the given number to a little-endian number.
	 * @tparam T Native type of the number.
	 * @param data Number to convert. 
	 * @return Converted number.
	 */
	template<typename T>
	static T toLittleEndian(T data);
private:
	Endian() = default;
};

template<typename T>
void Endian::fromBigEndian(const void* data, size_t size, void* buffer)
{
	const T* typedData = reinterpret_cast<const T*>(data);
	T* typedBuffer = reinterpret_cast<T*>(buffer);

	if (std::endian::big == std::endian::native)
	{
		std::copy(typedData, typedData + size, typedBuffer);
	}
	else
	{
		std::for_each(typedData, typedData + size, [&](T value)
		{
			std::reverse_copy(reinterpret_cast<uint8_t*>(&value), reinterpret_cast<uint8_t*>(&value) + sizeof(T), reinterpret_cast<uint8_t*>(typedBuffer));
			typedBuffer++;
		});
	}
}

template<typename T>
T Endian::fromBigEndian(const void* data)
{
	T value;
	Endian::fromBigEndian<T>(data, 1, &value);
	return value;
}

template<typename T>
T Endian::fromBigEndian(T data)
{
	return Endian::fromBigEndian<T>(&data);
}

template<typename T>
void Endian::fromLittleEndian(const void* data, size_t size, void* buffer)
{
	const T* typedData = reinterpret_cast<const T*>(data);
	T* typedBuffer = reinterpret_cast<T*>(buffer);

	if (std::endian::little == std::endian::native)
	{
		std::copy(typedData, typedData + size, typedBuffer);
	}
	else
	{
		std::for_each(typedData, typedData + size, [&](T value)
		{
			std::reverse_copy(reinterpret_cast<uint8_t*>(&value), reinterpret_cast<uint8_t*>(&value) + sizeof(T), reinterpret_cast<uint8_t*>(typedBuffer));
			typedBuffer++;
		});
	}
}

template<typename T>
T Endian::fromLittleEndian(const void* data)
{
	T value;
	Endian::fromLittleEndian<T>(data, 1, &value);
	return value;
}

template<typename T>
T Endian::fromLittleEndian(T data)
{
	return Endian::fromLittleEndian<T>(&data);
}

template<typename T>
void Endian::toBigEndian(const void* data, size_t size, void* buffer)
{
	Endian::fromBigEndian<T>(data, size, buffer);
}

template<typename T>
void Endian::toBigEndian(T data, void* buffer)
{
	Endian::toBigEndian<T>(&data, 1, buffer);
}

template<typename T>
T Endian::toBigEndian(T data)
{
	T value;
	Endian::toBigEndian<T>(data, &value);
	return value;
}

template<typename T>
void Endian::toLittleEndian(const void* data, size_t size, void* buffer)
{
	Endian::fromLittleEndian<T>(data, size, buffer);
}

template<typename T>
void Endian::toLittleEndian(T data, void* buffer)
{
	Endian::toLittleEndian<T>(&data, 1, buffer);
}

template<typename T>
T Endian::toLittleEndian(T data)
{
	T value;
	Endian::toLittleEndian<T>(data, &value);
	return value;
}

}  // namespace semf
#endif  // PROCESSING_ENDIAN_ENDIAN_H_
