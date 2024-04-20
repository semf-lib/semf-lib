/**
 * @file lastinbuffer.h
 * @date 13.03.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_UTILS_CORE_BUFFER_LASTINBUFFER_H_
#define SEMF_UTILS_CORE_BUFFER_LASTINBUFFER_H_

#include <semf/utils/core/buffer/buffer.h>

namespace semf
{
/**
 * @brief The \c LastInBuffer class implements a kind of circular buffer.
 *
 * Compared to \c RingBuffer, \c LastInBuffer has no write limitation:
 * Always the oldest data will be overwritten if the buffer is full.
 *
 * The data base is an array with the data type defined by the template parameter \c T.
 * To use the \c LastInBuffer set the array in the constructor or by the function \c setData().
 *
 * The data can be accessed by the functions
 * - \c data() to get the pointer to the data array,
 * - \c at() to get the element on a specific position and
 *
 * The data can be stored in the buffer by the function \c put().
 *
 * See also \c Buffer, \c LastInDmaBuffer \c RingBuffer and \c Average.
 *
 * @note For using \c LastInBuffer a global \c CriticalSection object is required.
 *
 * @tparam Type of data to handle.
 */
template <typename T>
class LastInBuffer : public Buffer<T>
{
public:
	using Buffer<T>::Buffer;
	virtual ~LastInBuffer() = default;

	/**
	 * @brief Writes data in the buffer.
	 * @param data Data to write in the buffer.
	 */
	virtual void put(T data);
	/**
	 * @brief Fills the whole buffer with the same object content.
	 * @param data The data which should be written into the whole buffer.
	 */
	void fill(T data);
	/**
	 * @brief Returns the entry from a specific position in the buffer.
	 * @param pos Position in the buffer.
	 * @return Entry.
	 */
	virtual T& operator[](size_t pos) const;
	/**
	 * @brief Sets the write position of the buffer.
	 * @param pos Write position.
	 */
	void setPos(size_t pos);
	/**
	 * @brief Returns the current write position.
	 * @return Position the next data will be written to.
	 */
	size_t pos() const;

private:
	/**Current write position in the buffer*/
	size_t m_pos = 0;
};

template <typename T>
void LastInBuffer<T>::put(T data)
{
	CriticalSection::enter();
	this->data()[m_pos++] = data;
	m_pos %= this->size();
	CriticalSection::exit();
}

template <typename T>
void LastInBuffer<T>::fill(T data)
{
	for (size_t i = 0; i < this->size(); i++)
	{
		CriticalSection::enter();
		this->data()[i] = data;
		CriticalSection::exit();
	}
}

template <typename T>
T& LastInBuffer<T>::operator[](size_t pos) const
{
	pos %= this->size();

	CriticalSection::enter();
	T& data = m_pos >= pos + 1 ? this->data()[m_pos - (pos + 1)] : this->data()[(this->size() + m_pos) - (pos + 1)];
	CriticalSection::exit();
	return data;
}

template <typename T>
void LastInBuffer<T>::setPos(size_t pos)
{
	m_pos = pos % this->size();
}

template <typename T>
size_t LastInBuffer<T>::pos() const
{
	return m_pos;
}
} /* namespace semf */
#endif /* SEMF_UTILS_CORE_BUFFER_LASTINBUFFER_H_ */
