/**
 * @file ringbuffer.h
 * @date 23.04.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_UTILS_CORE_BUFFER_RINGBUFFER_H_
#define SEMF_UTILS_CORE_BUFFER_RINGBUFFER_H_

#include <semf/utils/core/buffer/buffer.h>

namespace semf
{
/**
 * @brief The \c RingBuffer class implements a classic circular buffer.
 *
 * The data base is an array with the data type defined by the template parameter \c T.
 * To use the \c RingBuffer set the array in the constructor or by the function \c setData().
 *
 * The data can be accessed by the functions
 * - \c data() to get the pointer to the data array,
 * - \c at() to get the element on a specific position and
 * - \c get() to get the oldest entry not read jet.
 *
 * The data can be stored in the buffer by the function \c put().
 *
 * See also \c Buffer, \c LastInBuffer, \c LastInDmaBuffer and \c Average.
 *
 * @note For using \c RingBuffer a global \c CriticalSection object is required.
 *
 * @attention If \c USE_SEMF_DEBUG is defined \c RingBuffer uses \c nullptr dereferencing instead of an \c error signal.
 *
 * @tparam Type of data to handle.
 */
template <typename T>
class RingBuffer : public Buffer<T>
{
public:
	using Buffer<T>::Buffer;

	/**
	 * @brief Writes data in the buffer.
	 * @param data Data to write into the buffer into the next free space.
	 */
	virtual void put(T data);
	/**
	 * @brief Returns the oldest value not read yet.
	 * @return Oldest entry not read yet.
	 * @note Using this function while \c USE_SEMF_DEBUG or \c DEBUG being defined will cause a boundary check. In case of failing the boundary check the
	 *       function will dereference a \c nullptr in order to crash the software forcefully.
	 */
	T& get();
	/**
	 * @brief Returns the entry from a specific position in the buffer.
	 * @param pos Position in the buffer.
	 * @return Entry.
	 * @note Using this function while \c USE_SEMF_DEBUG or \c DEBUG being defined will cause a boundary check. In case of failing the boundary check the
	 *       function will dereference a \c nullptr in order to crash the software forcefully.
	 */
	T& operator[](size_t pos) const override;
	/**
	 * @brief Returns if the buffer is full.
	 * @note If the buffer is full use \c get() to free a buffer entry place before \c put().
	 * @return \li true: Buffer array is full.
	 * 		   \li false: Buffer array is has still free space.
	 */
	bool isFull() const;
	/**
	 * @brief Returns if the buffer is empty.
	 * @note If the buffer is empty, use \c put() before \c put().
	 * @return \li true: No unread data available.
	 * 		   \li false: Unread data available.
	 */
	bool isEmpty() const;
	/**
	 * @brief Returns the number of entries stored in the buffer and not read yet.
	 * @return Number of entries.
	 */
	size_t count() const override;
	/**
	 * @brief Clears the buffer.
	 * @note The data in the base array will not be modified.
	 */
	void reset();

private:
	/**Is used for counting the data in the buffer*/
	size_t m_dataCounter = 0;
	/**For saving the current write position*/
	size_t m_writePos = 0;
	/**For saving the current read position */
	size_t m_readPos = 0;
};

template <typename T>
void RingBuffer<T>::put(T data)
{
	if (isFull())
	{
		return;
	}

	CriticalSection::enter();
	++m_dataCounter;
	m_writePos = (m_writePos + 1) % this->size();
	this->data()[m_writePos] = data;
	CriticalSection::exit();
}

template <typename T>
T& RingBuffer<T>::get()
{
#if defined(DEBUG) || defined(USE_SEMF_DEBUG)
	if (isEmpty())
	{
		T* p = nullptr;
		return *p;
	}
#endif

	CriticalSection::enter();
	--m_dataCounter;
	m_readPos = (m_readPos + 1) % this->size();
	T& data = this->data()[m_readPos];
	CriticalSection::exit();

	return data;
}

template <typename T>
T& RingBuffer<T>::operator[](size_t pos) const
{
#if defined(DEBUG) || defined(USE_SEMF_DEBUG)
	if (isEmpty() || pos > m_dataCounter)
	{
		T* p = nullptr;
		return *p;
	}
#endif

	CriticalSection::enter();
	size_t readPos = (m_readPos + pos + 1) % this->size();
	T& data = this->data()[readPos];
	CriticalSection::exit();

	return data;
}

template <typename T>
bool RingBuffer<T>::isFull() const
{
	CriticalSection::enter();
	if (m_dataCounter == this->size())
	{
		CriticalSection::exit();
		return true;
	}
	CriticalSection::exit();
	return false;
}

template <typename T>
bool RingBuffer<T>::isEmpty() const
{
	CriticalSection::enter();
	if (m_dataCounter == 0)
	{
		CriticalSection::exit();
		return true;
	}

	CriticalSection::exit();
	return false;
}

template <typename T>
size_t RingBuffer<T>::count() const
{
	CriticalSection::enter();
	size_t counter = m_dataCounter;
	CriticalSection::exit();

	return counter;
}

template <typename T>
void RingBuffer<T>::reset()
{
	CriticalSection::enter();
	m_readPos = 0;
	m_writePos = 0;
	m_dataCounter = 0;
	CriticalSection::exit();
}
} /* namespace semf */
#endif /* SEMF_UTILS_CORE_BUFFER_RINGBUFFER_H_ */
