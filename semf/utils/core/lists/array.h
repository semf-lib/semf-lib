/**
 * @file array.h
 * @date 27.04.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory
 */

#ifndef SEMF_UTILS_CORE_LISTS_ARRAY_H_
#define SEMF_UTILS_CORE_LISTS_ARRAY_H_

#include <cstddef>
#include <iterator>
#include <utility>

namespace semf
{
/**
 * @brief This is an interface to \c std::array with the benefit of knowing its size without using a template parameter so it can be used easier in object
 *        oriented code.
 * @tparam T Element type of the array.
 */
template <typename T>
class Array
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
	virtual ~Array() = default;

	/**
	 * @brief Returns a reference to the element at specified location \c pos, with bounds checking.
	 * @param pos Position of the element to return.
	 * @return Reference to the requested element.
	 * @throws std::out_of_range If <code>!(pos < size())</code>.
	 */
	virtual reference at(size_type pos) = 0;
	/**
	 * @brief Returns a reference to the element at specified location \c pos, with bounds checking.
	 * @param pos Position of the element to return.
	 * @return Reference to the requested element.
	 * @throws std::out_of_range If <code>!(pos < size())</code>.
	 */
	virtual const_reference at(size_type pos) const = 0;
	/**
	 * @brief Returns a reference to the element at specified location \c pos. No bounds checking is performed.
	 * @param pos Position of the element to return.
	 * @return Reference to the requested element.
	 */
	virtual reference operator[](size_type pos) = 0;
	/**
	 * @brief Returns a reference to the element at specified location \c pos. No bounds checking is performed.
	 * @param pos Position of the element to return.
	 * @return Reference to the requested element.
	 */
	virtual const_reference operator[](size_type pos) const = 0;
	/**
	 * @brief Returns a reference to the first element in the container. Calling \c front on an empty container is undefined.
	 * @return Reference to the first element.
	 */
	virtual reference front() = 0;
	/**
	 * @brief Returns a reference to the first element in the container. Calling \c front on an empty container is undefined.
	 * @return Reference to the first element.
	 */
	virtual const_reference front() const = 0;
	/**
	 * @brief Returns a reference to the last element in the container. Calling \c back on an empty container causes undefined behavior.
	 * @return Reference to the last element.
	 */
	virtual reference back() = 0;
	/**
	 * @brief Returns a reference to the last element in the container. Calling \c back on an empty container causes undefined behavior.
	 * @return Reference to the last element.
	 */
	virtual const_reference back() const = 0;
	/**
	 * @brief Returns pointer to the underlying array serving as element storage. The pointer is such that range <code>[data(); data() + size())</code> is
	 *        always a valid range, even if the container is empty ( \c data() is not dereferenceable in that case).
	 * @return Pointer to the underlying element storage. For non-empty containers, the returned pointer compares equal to the address of the first element.
	 * @note If \c size() is 0, \c data() may or may not return a null pointer.
	 */
	virtual iterator data() = 0;
	/**
	 * @brief Returns pointer to the underlying array serving as element storage. The pointer is such that range <code>[data(); data() + size())</code> is
	 *        always a valid range, even if the container is empty ( \c data() is not dereferenceable in that case).
	 * @return Pointer to the underlying element storage. For non-empty containers, the returned pointer compares equal to the address of the first element.
	 * @note If \c size() is 0, \c data() may or may not return a null pointer.
	 */
	virtual const T* data() const = 0;
	/**
	 * @brief Returns an iterator to the first element of the array. If the array is empty, the returned iterator will be equal to \c end().
	 * @return Iterator to the first element.
	 */
	virtual iterator begin() = 0;
	/**
	 * @brief Returns an iterator to the first element of the array. If the array is empty, the returned iterator will be equal to \c end().
	 * @return Iterator to the first element.
	 */
	virtual const_iterator begin() const = 0;
	/**
	 * @brief Returns an iterator to the first element of the array. If the array is empty, the returned iterator will be equal to \c end().
	 * @return Iterator to the first element.
	 */
	virtual const_iterator cbegin() const = 0;
	/**
	 * @brief Returns an iterator to the element following the last element of the array. This element acts as a placeholder; attempting to access it results
	 *        in undefined behavior.
	 * @return Iterator to the element following the last element.
	 */
	virtual iterator end() = 0;
	/**
	 * @brief Returns an iterator to the element following the last element of the array. This element acts as a placeholder; attempting to access it results
	 *        in undefined behavior.
	 * @return Iterator to the element following the last element.
	 */
	virtual const_iterator end() const = 0;
	/**
	 * @brief Returns an iterator to the element following the last element of the array. This element acts as a placeholder; attempting to access it results
	 *        in undefined behavior.
	 * @return Iterator to the element following the last element.
	 */
	virtual const_iterator cend() const = 0;
	/**
	 * @brief Returns a reverse iterator to the first element of the reversed array. It corresponds to the last element of the non-reversed array. If the
	 *        array is empty, the returned iterator is equal to \c rend().
	 * @return Reverse iterator to the first element.
	 */
	virtual reverse_iterator rbegin() = 0;
	/**
	 * @brief Returns a reverse iterator to the first element of the reversed array. It corresponds to the last element of the non-reversed array. If the
	 *        array is empty, the returned iterator is equal to \c rend().
	 * @return Reverse iterator to the first element.
	 */
	virtual const_reverse_iterator rbegin() const = 0;
	/**
	 * @brief Returns a reverse iterator to the first element of the reversed array. It corresponds to the last element of the non-reversed array. If the
	 *        array is empty, the returned iterator is equal to \c rend().
	 * @return Reverse iterator to the first element.
	 */
	virtual const_reverse_iterator crbegin() const = 0;
	/**
	 * @brief Returns a reverse iterator to the element following the last element of the reversed array. It corresponds to the element preceding the first
	 *        element of the non-reversed array. This element acts as a placeholder, attempting to access it results in undefined behavior.
	 * @return Reverse iterator to the element following the last element.
	 */
	virtual reverse_iterator rend() = 0;
	/**
	 * @brief Returns a reverse iterator to the element following the last element of the reversed array. It corresponds to the element preceding the first
	 *        element of the non-reversed array. This element acts as a placeholder, attempting to access it results in undefined behavior.
	 * @return Reverse iterator to the element following the last element.
	 */
	virtual const_reverse_iterator rend() const = 0;
	/**
	 * @brief Returns a reverse iterator to the element following the last element of the reversed array. It corresponds to the element preceding the first
	 *        element of the non-reversed array. This element acts as a placeholder, attempting to access it results in undefined behavior.
	 * @return Reverse iterator to the element following the last element.
	 */
	virtual const_reverse_iterator crend() const = 0;
	/**
	 * @brief Checks if the container has no elements, i.e. whether <code>begin() == end()</code>.
	 * @return \c true if the container is empty, \c false otherwise.
	 */
	virtual bool empty() const = 0;
	/**
	 * @brief Returns the number of elements in the container, i.e. <code>std::distance(begin(), end())</code>.
	 * @return The number of elements in the container.
	 */
	virtual size_type size() const = 0;
	/**
	 * @brief Returns the maximum number of elements the container is able to hold due to system or library implementation limitations, i.e.
	 *        <code>std::distance(begin(), end())</code> for the largest container.
	 * @return Maximum number of elements.
	 */
	virtual size_type max_size() const = 0;
	/**
	 * @brief Assigns the given value value to all elements in the container.
	 * @param value The value to assign to the elements.
	 */
	virtual void fill(const T& value) = 0;
	/**
	 * @brief Exchanges the contents of the container with those of other. Does not cause iterators and references to associate with the other container.
	 *        In order to succeed both array must be equally sized.
	 * @param other Container to exchange the contents with.
	 * @return \c true if successful, otherwise \c false.
	 */
	virtual bool swap(Array<T>& other) = 0;
};
}  // namespace semf
#endif  // SEMF_UTILS_CORE_LISTS_ARRAY_H_
