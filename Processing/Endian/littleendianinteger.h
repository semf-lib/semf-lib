/**
 * @file littleendianinteger.h
 * 
 * @date 29.07.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory
 */

#ifndef PROCESSING_ENDIAN_LITTLEENDIANINTEGER_H_
#define PROCESSING_ENDIAN_LITTLEENDIANINTEGER_H_

#include <Processing/stdworkarounds/algorithm>
#include <Processing/stdworkarounds/bit>
#include <limits>
#include <algorithm>
#include <cstdint>

#if __cplusplus >= 201703L
#define IF_CONSTEXPR if constexpr
#else
#define IF_CONSTEXPR if
#endif

namespace semf
{
/**
 * @brief Class for representing little-endian numbers.
 * @tparam T Native type of the number.
 */
template<typename T>
class LittleEndianInteger
{
public:
	/**
	 * @brief Constructor.
	 * @param value Value of the number.
	 */
	constexpr LittleEndianInteger(T value);
	/**
	 * @brief Returns the maximum (finite) value representable by the numeric type T.
	 * @return Maximum (finite) value.
	 */
	static LittleEndianInteger<T> max();
	/**
	 * @brief Returns the minimum (finite) value representable by the numeric type T.
	 * @return Minimum (finite) value.
	 */
	static LittleEndianInteger<T> min();
	/**
	 * @brief Returns a native representation of the stored number.
	 * @return Native representation of the stored number.
	 */
	T native() const;
	/**
	 * @brief Checks if \c this and \c other are unequal.
	 * @param other Other number.
	 * @return \c true if unequal.
	 */
	bool operator!=(LittleEndianInteger<T> other) const;
	/**
	 * @brief Calculates the modulo.
	 * @param i Divisor.
	 * @return Reference to \c this.
	 */
	LittleEndianInteger<T>& operator%=(T i);
	/**
	 * @brief Performes a bitwise AND.
	 * @param i Other number.
	 * @return Reference to \c this.
	 */
	LittleEndianInteger<T>& operator &=(T i);
	/**
	 * @brief Performes a multiplication.
	 * @param i Other number.
	 * @return Reference to \c this.
	 */
	LittleEndianInteger<T>& operator*=(T i);
	/**
	 * @brief Increments \c this (prefix) and returns a reference to \c this.
	 * @return Reference to \c this.
	 */
	LittleEndianInteger<T>& operator++();
	/**
	 * @brief Increments \c this (postfix) and returs a reference to \c this.
	 * @return Reference to \c this.
	 * @note This operator overload has the same behavior as the prefix operator.
	 */
	LittleEndianInteger<T>& operator++(int);
	/**
	 * @brief Performes an addition.
	 * @param i Other number.
	 * @return Reference to \c this.
	 */
	LittleEndianInteger<T>& operator+=(T i);
	/**
	 * @brief Decrements \c this (prefix) and returs a reference to \c this.
	 * @return Reference to \c this.
	 */
	LittleEndianInteger<T>& operator--();
	/**
	 * @brief Decrements \c this (postfix) and returs a reference to \c this.
	 * @return Reference to \c this.
	 * @note This operator overload has the same behavior as the prefix operator.
	 */
	LittleEndianInteger<T>& operator--(int);
	/**
	 * @brief Performes a substraction.
	 * @param i Subtrahend.
	 * @return Reference to \c this.
	 */
	LittleEndianInteger<T>& operator-=(T i);
	/**
	 * @brief Performes a division.
	 * @param i Divisor.
	 * @return Reference to \c this.
	 */
	LittleEndianInteger<T>& operator/=(T i);
	/**
	 * @brief Shift \c i bits left.
	 * @param i Bits to shift.
	 * @return Reference to \c this.
	 */
	LittleEndianInteger<T>& operator<<=(T i);
	/**
	 * @brief Assigns \c this with the value \c i.
	 * @param i Value to assign.
	 * @return Reference to *this.
	 */
	LittleEndianInteger<T>& operator=(T i);
	/**
	 * @brief Performes a bitwise XOR.
	 * @param i Other number.
	 * @return Reference to \c this.
	 */
	LittleEndianInteger<T>& operator^=(T i);
	/**
	 * @brief Checks for equality between \c this and \c other.
	 * @param other Number to compare to.
	 * @return \c true if equal.
	 */
	bool operator==(LittleEndianInteger<T> other) const;
	/**
	 * @brief Shift \c i bits right.
	 * @param i Bits to shift.
	 * @return Reference to \c this.
	 */
	LittleEndianInteger<T>& operator>>=(T i);
	/**
	 * @brief Performes a bitwise OR.
	 * @param i Other number.
	 * @return Reference to \c this.
	 */
	LittleEndianInteger<T>& operator|=(T i);
private:
	/**
	 * @brief Sets m_value to the given native number.
	 * @param value New value.
	 */
	void setValue(T value);
	/**Little-endian value.*/
	T m_value;
};

template<typename T>
constexpr LittleEndianInteger<T>::LittleEndianInteger(T value)
: m_value
	{
		std::endian::big == std::endian::native ?
			reverse<T>(reinterpret_cast<uint8_t*>(&value), reinterpret_cast<uint8_t*>(&value) + sizeof(T)) :
			value
	}
{}

template<typename T>
LittleEndianInteger<T> LittleEndianInteger<T>::max()
{
	return std::numeric_limits<T>::max();
}

template<typename T>
LittleEndianInteger<T> LittleEndianInteger<T>::min()
{
	return std::numeric_limits<T>::min();
}

template<typename T>
T LittleEndianInteger<T>::native() const
{
	T value = m_value;
	IF_CONSTEXPR(std::endian::big == std::endian::native)
	{
		std::reverse(reinterpret_cast<uint8_t*>(&value), reinterpret_cast<uint8_t*>(&value) + sizeof(T));
	}
	return value;
}

template<typename T>
bool LittleEndianInteger<T>::operator!=(LittleEndianInteger<T> other) const
{
	return m_value != other.m_value;
}

template<typename T>
LittleEndianInteger<T>& LittleEndianInteger<T>::operator%=(T i)
{
	T value = native();
	value %= i;
	setValue(value);
	return *this;
}

template<typename T>
LittleEndianInteger<T>& LittleEndianInteger<T>::operator&=(T i)
{
	T value = native();
	value &= i;
	setValue(value);
	return *this;
}

template<typename T>
LittleEndianInteger<T>& LittleEndianInteger<T>::operator*=(T i)
{
	T value = native();
	value *= i;
	setValue(value);
	return *this;
}

template<typename T>
LittleEndianInteger<T>& LittleEndianInteger<T>::operator++()
{
	T value = native();
	value++;
	setValue(value);
	return *this;
}

template<typename T>
LittleEndianInteger<T>& LittleEndianInteger<T>::operator++ (int i)
{
	(void)i;
	return this->operator++();
}

template<typename T>
LittleEndianInteger<T>& LittleEndianInteger<T>::operator+=(T i)
{
	T value = native();
	value += i;
	setValue(value);
	return *this;
}

template<typename T>
LittleEndianInteger<T>& LittleEndianInteger<T>::operator--()
{
	T value = native();
	value--;
	setValue(value);
	return *this;
}

template<typename T>
LittleEndianInteger<T>& LittleEndianInteger<T>::operator-- (int i)
{
	(void)i;
	return this->operator--();
}

template<typename T>
LittleEndianInteger<T>& LittleEndianInteger<T>::operator-=(T i)
{
	T value = native();
	value -= i;
	setValue(value);
	return *this;
}

template<typename T>
LittleEndianInteger<T>& LittleEndianInteger<T>::operator/=(T i)
{
	T value = native();
	value %= i;
	setValue(value);
	return *this;
}

template<typename T>
LittleEndianInteger<T>& LittleEndianInteger<T>::operator<<=(T i)
{
	T value = native();
	value <<= i;
	setValue(value);
	return *this;
}

template<typename T>
LittleEndianInteger<T>& LittleEndianInteger<T>::operator=(T i)
{
	T value = native();
	value = i;
	setValue(value);
	return *this;
}

template<typename T>
LittleEndianInteger<T>& LittleEndianInteger<T>::operator^=(T i)
{
	T value = native();
	value ^= i;
	setValue(value);
	return *this;
}

template<typename T>
bool LittleEndianInteger<T>::operator==(LittleEndianInteger<T> other) const
{
	return m_value == other.m_value;
}

template<typename T>
LittleEndianInteger<T>& LittleEndianInteger<T>::operator>>=(T i)
{
	T value = native();
	value >>= i;
	setValue(value);
	return *this;
}

template<typename T>
LittleEndianInteger<T>& LittleEndianInteger<T>::operator|=(T i)
{
	T value = native();
	value |= i;
	setValue(value);
	return *this;
}

template<typename T>
void LittleEndianInteger<T>::setValue(T value)
{
	IF_CONSTEXPR(std::endian::big == std::endian::native)
	{
		std::reverse(reinterpret_cast<uint8_t*>(&value), reinterpret_cast<uint8_t*>(&value) + sizeof(T));
	}
	m_value = value;
}

}  // namespace semf
#endif  // PROCESSING_ENDIAN_LITTLEENDIANINTEGER_H_
