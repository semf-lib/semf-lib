/**
 * @file stdarray.h
 * @date 29.04.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory
 */

#ifndef SEMF_UTILS_CORE_LISTS_STDARRAY_H_
#define SEMF_UTILS_CORE_LISTS_STDARRAY_H_

#include <semf/utils/core/lists/array.h>
#include <array>
#include <utility>

namespace semf
{
/**
 * @brief This class can be used for creating fixed sized array object. It implements the \c semf::Array interface in order to pass objects without the use
 *        of a template. It uses the implementation of \c std::array.
 * @tparam T Element type.
 * @tparam N Array Size.
 */
template <typename T, size_t N>
class StdArray : private virtual std::array<T, N>, public Array<T>
{
public:
	/**Value type.*/
	using value_type = T;
	/**Size type.*/
	using size_type = std::size_t;
	/**Difference type.*/
	using difference_type = std::ptrdiff_t;
	/**Reference type.*/
	using reference = value_type&;
	/**Const reference type.*/
	using const_reference = const value_type&;
	/**Iterator type.*/
	using iterator = T*;
	/**Const iterator type.*/
	using const_iterator = const T*;
	/**Reverse iterator type.*/
	using reverse_iterator = std::reverse_iterator<iterator>;
	/**Const reverse iterator type.*/
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	/**
	 * @brief Constructs a new StdArray object using an initializer list.
	 * @tparam U Initializer list type.
	 * @param values Initial values of the array.
	 */
	template <class... U>
	StdArray(U&&... values);  // NOLINT
	virtual ~StdArray() = default;

	reference at(size_type pos) override;
	const_reference at(size_type pos) const override;
	reference operator[](size_type pos) override;
	const_reference operator[](size_type pos) const override;
	reference front() override;
	const_reference front() const override;
	reference back() override;
	const_reference back() const override;
	T* data() noexcept override;
	const T* data() const noexcept override;
	iterator begin() noexcept override;
	const_iterator begin() const noexcept override;
	const_iterator cbegin() const noexcept override;
	iterator end() noexcept override;
	const_iterator end() const noexcept override;
	const_iterator cend() const noexcept override;
	reverse_iterator rbegin() noexcept override;
	const_reverse_iterator rbegin() const noexcept override;
	const_reverse_iterator crbegin() const noexcept override;
	reverse_iterator rend() noexcept override;
	const_reverse_iterator rend() const noexcept override;
	const_reverse_iterator crend() const noexcept override;
	bool empty() const noexcept override;
	size_type size() const noexcept override;
	size_type max_size() const noexcept override;
	void fill(const T& value) override;
	bool swap(Array<T>& other) override;
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
template <typename T, size_t N>
template <class... U>
StdArray<T, N>::StdArray(U&&... values)
: std::array<T, N>{{std::forward<U>(values)...}}
{
}

template <typename T, size_t N>
typename StdArray<T, N>::reference StdArray<T, N>::at(StdArray<T, N>::size_type pos)
{
	return std::array<T, N>::at(pos);
}

template <typename T, size_t N>
typename StdArray<T, N>::const_reference StdArray<T, N>::at(StdArray<T, N>::size_type pos) const
{
	return std::array<T, N>::at(pos);
}

template <typename T, size_t N>
typename StdArray<T, N>::reference StdArray<T, N>::operator[](StdArray<T, N>::size_type pos)
{
	return std::array<T, N>::operator[](pos);
}

template <typename T, size_t N>
typename StdArray<T, N>::const_reference StdArray<T, N>::operator[](StdArray<T, N>::size_type pos) const
{
	return std::array<T, N>::at(pos);
}

template <typename T, size_t N>
typename StdArray<T, N>::reference StdArray<T, N>::front()
{
	return std::array<T, N>::front();
}

template <typename T, size_t N>
typename StdArray<T, N>::const_reference StdArray<T, N>::front() const
{
	return std::array<T, N>::front();
}

template <typename T, size_t N>
typename StdArray<T, N>::reference StdArray<T, N>::back()
{
	return std::array<T, N>::back();
}

template <typename T, size_t N>
typename StdArray<T, N>::const_reference StdArray<T, N>::back() const
{
	return std::array<T, N>::back();
}

template <typename T, size_t N>
T* StdArray<T, N>::data() noexcept
{
	return std::array<T, N>::data();
}

template <typename T, size_t N>
const T* StdArray<T, N>::data() const noexcept
{
	return std::array<T, N>::data();
}

template <typename T, size_t N>
typename StdArray<T, N>::iterator StdArray<T, N>::begin() noexcept
{
	return std::array<T, N>::begin();
}

template <typename T, size_t N>
typename StdArray<T, N>::const_iterator StdArray<T, N>::begin() const noexcept
{
	return std::array<T, N>::begin();
}

template <typename T, size_t N>
typename StdArray<T, N>::const_iterator StdArray<T, N>::cbegin() const noexcept
{
	return std::array<T, N>::cbegin();
}

template <typename T, size_t N>
typename StdArray<T, N>::iterator StdArray<T, N>::end() noexcept
{
	return std::array<T, N>::end();
}

template <typename T, size_t N>
typename StdArray<T, N>::const_iterator StdArray<T, N>::end() const noexcept
{
	return std::array<T, N>::end();
}

template <typename T, size_t N>
typename StdArray<T, N>::const_iterator StdArray<T, N>::cend() const noexcept
{
	return std::array<T, N>::cend();
}

template <typename T, size_t N>
typename StdArray<T, N>::reverse_iterator StdArray<T, N>::rbegin() noexcept
{
	return std::array<T, N>::rbegin();
}

template <typename T, size_t N>
typename StdArray<T, N>::const_reverse_iterator StdArray<T, N>::rbegin() const noexcept
{
	return std::array<T, N>::rbegin();
}

template <typename T, size_t N>
typename StdArray<T, N>::const_reverse_iterator StdArray<T, N>::crbegin() const noexcept
{
	return std::array<T, N>::crbegin();
}

template <typename T, size_t N>
typename StdArray<T, N>::reverse_iterator StdArray<T, N>::rend() noexcept
{
	return std::array<T, N>::rend();
}

template <typename T, size_t N>
typename StdArray<T, N>::const_reverse_iterator StdArray<T, N>::rend() const noexcept
{
	return std::array<T, N>::rend();
}

template <typename T, size_t N>
typename StdArray<T, N>::const_reverse_iterator StdArray<T, N>::crend() const noexcept
{
	return std::array<T, N>::crend();
}

template <typename T, size_t N>
bool StdArray<T, N>::empty() const noexcept
{
	return std::array<T, N>::empty();
}

template <typename T, size_t N>
typename StdArray<T, N>::size_type StdArray<T, N>::size() const noexcept
{
	return std::array<T, N>::size();
}

template <typename T, size_t N>
typename StdArray<T, N>::size_type StdArray<T, N>::max_size() const noexcept
{
	return std::array<T, N>::max_size();
}

template <typename T, size_t N>
void StdArray<T, N>::fill(const T& value)
{
	return std::array<T, N>::fill(value);
}

template <typename T, size_t N>
bool StdArray<T, N>::swap(Array<T>& other)
{
	if (size() == other.size())
	{
		std::array<T, N>::swap(*reinterpret_cast<StdArray<T, N>*>(&other));
		return true;
	}
	return false;
}
}  // namespace semf
#endif  // DOXYGEN_SHOULD_SKIP_THIS
#endif  // SEMF_UTILS_CORE_LISTS_STDARRAY_H_
