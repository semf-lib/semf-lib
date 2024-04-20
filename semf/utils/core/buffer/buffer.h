/**
 * @file buffer.h
 * @date 22.08.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_UTILS_CORE_BUFFER_BUFFER_H_
#define SEMF_UTILS_CORE_BUFFER_BUFFER_H_

#include <semf/utils/core/debug.h>
#include <semf/system/criticalsection.h>
#include <cstdint>
#include <cstddef>

namespace semf
{
/**
 * @brief \c Buffer is a base class for buffer implementations.
 *
 * The data base is an array with the data type defined by the template parameter \c T.
 * To use the \c Buffer set the array in the constructor or by the function \c setData().
 *
 * The data can be accessed by the functions
 * - \c data() to get the pointer to the data array and
 * - \c at() to get the element on a specific position.
 *
 * \c Buffer itself has no function to put data in the buffer.
 *
 * See also \c RingBuffer, \c LastInBuffer, \c LastInDmaBuffer and \c Average.
 *
 * @note For using \c RingBuffer a global \c CriticalSection object is required.
 *
 * @attention If \c USE_SEMF_DEBUG or \c DEBUG is defined \c Buffer uses \c nullptr dereferencing instead of an \c error signal.
 *
 * @tparam T Data type
 */
template <typename T>
class Buffer
{
public:
	Buffer() = default;
	/**
	 * @brief Constructor with data array initialization.
	 * @param buffer Pointer to the array to store data in.
	 * @param bufferSize The size of the data array.
	 */
	Buffer(T buffer[], size_t bufferSize);
	virtual ~Buffer() = default;

	/**
	 * @brief Sets the data array.
	 * @param buffer Pointer to the array to store data in.
	 * @param bufferSize The size of the data array.
	 */
	void setBuffer(T buffer[], size_t bufferSize);
	/**
	 * @brief Returns the entry from a specific position in the buffer.
	 * @param pos Position in the buffer.
	 * @return Data entry.
	 * @note Using this function while \c USE_SEMF_DEBUG or \c DEBUG being defined will cause a boundary check. In case of failing the boundary check the
	 *       function will dereference a \c nullptr in order to crash the software forcefully.
	 */
	virtual T& operator[](size_t pos) const;
	/**
	 * @brief Returns the size of the buffer.
	 * @return Buffer size.
	 */
	size_t size() const;
	/**
	 * @brief Returns the number of entries stored in the buffer.
	 * @note In this basic implementation \c count() returns \c size().
	 * @return Number of entries.
	 */
	virtual size_t count() const;
	/**
	 * @brief Returns the pointer to the first element of the data array.
	 * @return Pointer to the data array.
	 */
	T* data() const;

private:
	/**The buffer for saving the data in*/
	T* m_data = nullptr;
	/**Size of the buffer*/
	size_t m_size = 0;
};

template <typename T>
Buffer<T>::Buffer(T buffer[], size_t bufferSize)
: m_data(buffer),
  m_size(bufferSize)
{
}

template <typename T>
void Buffer<T>::setBuffer(T buffer[], size_t bufferSize)
{
	if (buffer != nullptr && bufferSize > 0)
	{
		CriticalSection::enter();
		m_data = buffer;
		m_size = bufferSize;
		CriticalSection::exit();
	}
}

template <typename T>
T& Buffer<T>::operator[](size_t pos) const
{
#if defined(DEBUG) || defined(USE_SEMF_DEBUG)
	if (pos >= m_size)
	{
		T* p = nullptr;
		return *p;
	}
#endif
	CriticalSection::enter();
	T& entry = m_data[pos];
	CriticalSection::exit();
	return entry;
}

template <typename T>
size_t Buffer<T>::size() const
{
	return m_size;
}

template <typename T>
size_t Buffer<T>::count() const
{
	return m_size;
}

template <typename T>
T* Buffer<T>::data() const
{
	return m_data;
}
} /* namespace semf */
#endif /* SEMF_UTILS_CORE_BUFFER_BUFFER_H_ */
