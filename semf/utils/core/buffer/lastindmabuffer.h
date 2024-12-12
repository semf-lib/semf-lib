/**
 * @file lastindmabuffer.h
 * @date 13.03.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_UTILS_CORE_BUFFER_LASTINDMABUFFER_H_
#define SEMF_UTILS_CORE_BUFFER_LASTINDMABUFFER_H_

#include <semf/utils/core/buffer/lastinbuffer.h>

namespace semf
{
/**
 * @brief The \c LastInDmaBuffer class implements a kind of circular buffer.
 *
 * \c LastInDmaBuffer handles multiple \c LastInBuffer in one data array.
 *
 * The data base is an array with the data type defined by the template parameter \c T.
 * To use the \c LastInDmaBuffer set the array in the constructor or by the function \c setData().
 *
 * The data can be accessed by the functions
 * - \c data() to get the pointer to the data array,
 * - \c at() to get the element on a specific position and
 *
 * The data can be stored in the buffer by the function \c put().
 *
 * See also \c Buffer, \c LastInBuffer \c RingBuffer and \c Average.
 *
 * @note For using \c LastInDmaBuffer a global \c CriticalSection object is required.
 *
 * @tparam Type of data to handle.
 */
template <typename T>
class LastInDmaBuffer : public LastInBuffer<T>
{
public:
	using LastInBuffer<T>::LastInBuffer;

	/**
	 * @brief Write data in the buffer.
	 * @param data Data to write in the buffer.
	 */
	void put(T data) override;
	/**
	 * @brief Returns the entry from a specific position in the buffer.
	 * @param pos Position in the buffer.
	 * @return The Entry.
	 */
	T& operator[](size_t pos) const override;
	/**
	 * @brief Set the stride between to entries of the same input.
	 * @note For example: Use a dma sampling three different data, the stride between two entries in this buffer is 3.
	 * @param stride Stride between to entries.
	 */
	void setStride(size_t stride);
	/**
	 * @brief Returns the size of the buffer.
	 * @return Buffer size.
	 */
	size_t size() const;
	/**
	 * @brief Returns the number of entries stored in the buffer.
	 * @note For \c LastInDmaBuffer \c count() returns \c size().
	 * @return Number of entries.
	 */
	size_t count() const override;

private:
	/**Stride between two entries*/
	size_t m_stride = 0;
};

template <typename T>
void LastInDmaBuffer<T>::put(T data)
{
	LastInBuffer<T>::put(data);
	CriticalSection::enter();
	LastInBuffer<T>::setPos(this->pos() + m_stride - 1);
	CriticalSection::exit();
}

template <typename T>
T& LastInDmaBuffer<T>::operator[](size_t pos) const
{
	return LastInBuffer<T>::operator[]((pos + 1) * (m_stride)-1);
}

template <typename T>
void LastInDmaBuffer<T>::setStride(size_t stride)
{
	m_stride = stride;
}

template <typename T>
size_t LastInDmaBuffer<T>::size() const
{
	return LastInBuffer<T>::size() / (m_stride);
}

template <typename T>
size_t LastInDmaBuffer<T>::count() const
{
	return size();
}
} /* namespace semf */
#endif /* SEMF_UTILS_CORE_BUFFER_LASTINDMABUFFER_H_ */
