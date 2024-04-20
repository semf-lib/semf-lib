/**
 * @file bigendianinteger.h
 * @date 28.07.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory
 */

#ifndef SEMF_UTILS_PROCESSING_ENDIAN_BIGENDIANINTEGER_H_
#define SEMF_UTILS_PROCESSING_ENDIAN_BIGENDIANINTEGER_H_

#include <semf/utils/processing/stdworkarounds/algorithm>
#include <semf/utils/processing/stdworkarounds/bit>
#include <algorithm>
#include <cstdint>
#include <limits>
#include <type_traits>

#if __cplusplus >= 201703L
#define IF_CONSTEXPR if constexpr
#else
#define IF_CONSTEXPR if
#endif

namespace semf
{
/**
 * @brief Class for representing big-endian numbers.
 * @tparam T Native type of the number.
 */
template <typename T>
class BigEndianInteger
{
public:
	/**
	 * @brief Constructor.
	 * @param value Value of the number.
	 */
	constexpr BigEndianInteger(T value);
	virtual ~BigEndianInteger() = default;

	/**
	 * @brief Returns the maximum (finite) value representable by the numeric type T.
	 * @return Maximum (finite) value.
	 */
	static BigEndianInteger<T> max();
	/**
	 * @brief Returns the minimum (finite) value representable by the numeric type T.
	 * @return Minimum (finite) value.
	 */
	static BigEndianInteger<T> min();
	/**
	 * @brief Returns a native representation of the stored number.
	 * @return Native representation of the stored number.
	 */
	T native() const;
	/**
	 * @brief Check if \c this and \c other are unequal.
	 * @param other Other number.
	 * @return \c true if unequal.
	 */
	bool operator!=(BigEndianInteger<T> other) const;
	/**
	 * @brief Calculates the modulo.
	 * @param i Divisor.
	 * @return Reference to \c this.
	 */
	BigEndianInteger<T>& operator%=(T i);
	/**
	 * @brief Performs a bitwise AND.
	 * @param i Other number.
	 * @return Reference to \c this.
	 */
	BigEndianInteger<T>& operator&=(T i);
	/**
	 * @brief Performs a multiplication.
	 * @param i Other number.
	 * @return Reference to \c this.
	 */
	BigEndianInteger<T>& operator*=(T i);
	/**
	 * @brief Increments \c this (prefix) and returns a reference to \c this.
	 * @return Reference to \c this.
	 */
	BigEndianInteger<T>& operator++();
	/**
	 * @brief Increments \c this (postfix) and returns a reference to \c this.
	 * @param  i Number 1.
	 * @note This operator overload has the same behavior as the prefix operator.
	 * @return Reference to \c this.
	 */
	BigEndianInteger<T>& operator++(int i);
	/**
	 * @brief Performs an addition.
	 * @param i Other number.
	 * @return Reference to \c this.
	 */
	BigEndianInteger<T>& operator+=(T i);
	/**
	 * @brief Decrements \c this (prefix) and returns a reference to \c this.
	 * @return Reference to \c this.
	 */
	BigEndianInteger<T>& operator--();
	/**
	 * @brief Decrements \c this (postfix) and returns a reference to \c this.
	 * @note This operator overload has the same behavior as the prefix operator.
	 * @param  i Number 1.
	 * @return Reference to \c this.
	 */
	BigEndianInteger<T>& operator--(int i);
	/**
	 * @brief Performs a subtraction.
	 * @param i Subtrahend.
	 * @return Reference to \c this.
	 */
	BigEndianInteger<T>& operator-=(T i);
	/**
	 * @brief Performs a division.
	 * @param i Divisor.
	 * @return Reference to \c this.
	 */
	BigEndianInteger<T>& operator/=(T i);
	/**
	 * @brief Shift \c i bits left.
	 * @param i Bits to shift.
	 * @return Reference to \c this.
	 */
	BigEndianInteger<T>& operator<<=(T i);
	/**
	 * @brief Assigns \c this with the value \c i.
	 * @param i Value to assign.
	 * @return Reference to *this.
	 */
	BigEndianInteger<T>& operator=(T i);
	/**
	 * @brief Performs a bitwise XOR.
	 * @param i Other number.
	 * @return Reference to \c this.
	 */
	BigEndianInteger<T>& operator^=(T i);
	/**
	 * @brief Checks for equality between \c this and \c other.
	 * @param other Number to compare to.
	 * @return \c true if equal.
	 */
	bool operator==(BigEndianInteger<T> other) const;
	/**
	 * @brief Shift \c i bits right.
	 * @param i Bits to shift.
	 * @return Reference to \c this.
	 */
	BigEndianInteger<T>& operator>>=(T i);
	/**
	 * @brief Performs a bitwise OR.
	 * @param i Other number.
	 * @return Reference to \c this.
	 */
	BigEndianInteger<T>& operator|=(T i);

private:
	/**
	 * @brief Sets m_value to the given native number.
	 * @param value New value.
	 */
	void setValue(T value);
	/**Big-endian value.*/
	T m_value;
};

template <typename T>
constexpr BigEndianInteger<T>::BigEndianInteger(T value)
: m_value{std::endian::little == std::endian::native ? reverse<T>(reinterpret_cast<uint8_t*>(&value), reinterpret_cast<uint8_t*>(&value) + sizeof(T)) : value}
{
}

template <typename T>
BigEndianInteger<T> BigEndianInteger<T>::max()
{
	BigEndianInteger<T> value(std::numeric_limits<T>::max());
	return value;
}

template <typename T>
BigEndianInteger<T> BigEndianInteger<T>::min()
{
	BigEndianInteger<T> value(std::numeric_limits<T>::min());
	return value;
}

template <typename T>
T BigEndianInteger<T>::native() const
{
	T native = m_value;
	IF_CONSTEXPR(std::endian::little == std::endian::native)
	{
		std::reverse(reinterpret_cast<uint8_t*>(&native), reinterpret_cast<uint8_t*>(&native) + sizeof(T));
	}
	return native;
}

template <typename T>
bool BigEndianInteger<T>::operator!=(BigEndianInteger<T> other) const
{
	return m_value != other.m_value;
}

template <typename T>
BigEndianInteger<T>& BigEndianInteger<T>::operator%=(T i)
{
	T nativeValue = native();
	nativeValue %= i;
	setValue(nativeValue);
	return *this;
}

template <typename T>
void BigEndianInteger<T>::setValue(T value)
{
	m_value = value;
	IF_CONSTEXPR(std::endian::little == std::endian::native)
	{
		std::reverse(reinterpret_cast<uint8_t*>(&m_value), reinterpret_cast<uint8_t*>(&m_value) + sizeof(T));
	}
}

template <typename T>
BigEndianInteger<T>& BigEndianInteger<T>::operator&=(T i)
{
	T nativeValue = native();
	nativeValue &= i;
	setValue(nativeValue);
	return *this;
}

template <typename T>
BigEndianInteger<T>& BigEndianInteger<T>::operator*=(T i)
{
	T nativeValue = native();
	nativeValue *= i;
	setValue(nativeValue);
	return *this;
}

template <typename T>
BigEndianInteger<T>& BigEndianInteger<T>::operator++()
{
	T nativeValue = native();
	nativeValue++;
	setValue(nativeValue);
	return *this;
}

template <typename T>
BigEndianInteger<T>& BigEndianInteger<T>::operator++(int i)
{
	(void)i;
	return this->operator++();
}

template <typename T>
BigEndianInteger<T>& BigEndianInteger<T>::operator+=(T i)
{
	T nativeValue = native();
	nativeValue += i;
	setValue(nativeValue);
	return *this;
}

template <typename T>
BigEndianInteger<T>& BigEndianInteger<T>::operator--()
{
	T nativeValue = native();
	nativeValue--;
	setValue(nativeValue);
	return *this;
}

template <typename T>
BigEndianInteger<T>& BigEndianInteger<T>::operator--(int i)
{
	(void)i;
	return this->operator--();
}

template <typename T>
BigEndianInteger<T>& BigEndianInteger<T>::operator-=(T i)
{
	T nativeValue = native();
	nativeValue -= i;
	setValue(nativeValue);
	return *this;
}

template <typename T>
BigEndianInteger<T>& BigEndianInteger<T>::operator/=(T i)
{
	T nativeValue = native();
	nativeValue /= i;
	setValue(nativeValue);
	return *this;
}

template <typename T>
BigEndianInteger<T>& BigEndianInteger<T>::operator<<=(T i)
{
	T nativeValue = native();
	nativeValue <<= i;
	setValue(nativeValue);
	return *this;
}

template <typename T>
BigEndianInteger<T>& BigEndianInteger<T>::operator=(T i)
{
	setValue(i);
	return *this;
}

template <typename T>
bool BigEndianInteger<T>::operator==(BigEndianInteger<T> other) const
{
	return m_value == other.m_value;
}

template <typename T>
BigEndianInteger<T>& BigEndianInteger<T>::operator>>=(T i)
{
	T nativeValue = native();
	nativeValue >>= i;
	setValue(nativeValue);
	return *this;
}

template <typename T>
BigEndianInteger<T>& BigEndianInteger<T>::operator^=(T i)
{
	T nativeValue = native();
	nativeValue ^= i;
	setValue(nativeValue);
	return *this;
}

template <typename T>
BigEndianInteger<T>& BigEndianInteger<T>::operator|=(T i)
{
	T nativeValue = native();
	nativeValue |= i;
	setValue(nativeValue);
	return *this;
}
}  // namespace semf
#endif  // SEMF_UTILS_PROCESSING_ENDIAN_BIGENDIANINTEGER_H_
