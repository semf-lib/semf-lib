/**
 * @file linkedlist.h
 * @date 19.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_UTILS_CORE_LISTS_LINKEDLIST_H_
#define SEMF_UTILS_CORE_LISTS_LINKEDLIST_H_

#include <iterator>
#include <cstdint>
#include <cstddef>

namespace semf
{
/**
 * @brief LinkedList is an managed double linked list implementation.
 *
 * It is similar to std::list, but works without dynamic storage allocation.
 * Every element in the list has to inherit from \c LinkedList::Node and
 * in case has the information of the \c previous() and the \c next() node.
 *
 * Like std::list \c LinkedList also is a sequence of elements that allow
 * constant time insert and erase operations anywhere within the sequence,
 * and iteration in both directions.
 *
 * List is implemented as doubly-linked lists. Double linked lists can store
 * each of the elements they contain in different and unrelated storage locations.
 * The ordering is kept internally by the association to each element of a link to
 * the element preceding it and a link to the element following it.
 *
 * @attention A node is only allowed to be part of one list. Putting a node into
 * a second list changes the references in the element. That causes to
 * undefined behavior by iterating through the lists.
 *
 * @tparam T class type inherits from \c LinkedList::Node
 */
template <class T>
class LinkedList
{
public:
	/**
	 * @brief Implements the \c previous() and \c next() functionality for
	 * every element in a list.
	 *
	 * A class type using \c LinkedList has to inherit from this class.
	 */
	class Node
	{
	public:
		virtual ~Node() = default;

		/**
		 * @brief Returns a pointer to the next element in a list.
		 * @attention The last element in a list will return \c nullptr.
		 * @return The next element in a list.
		 */
		T* next() const
		{
			return m_next;
		}
		/**
		 * @brief Sets a pointer to the next element in a list.
		 * @attention Do not use outside LinkedList. That leads to undefined behavior.
		 * @param next The next element in a list.
		 */
		void setNext(T* next)
		{
			m_next = next;
		}
		/**
		 * @brief Returns a pointer to the previous element in a list.
		 * @attention The first element in a list will return \c nullptr.
		 * @return The previous element in a list.
		 */
		T* previous() const
		{
			return m_previous;
		}
		/**
		 * @brief Sets a pointer to the previous element in a list.
		 * @attention Do not use outside LinkedList. That leads to undefined behavior.
		 * @param previous The previous element in a list.
		 */
		void setPrevious(T* previous)
		{
			m_previous = previous;
		}
		/**
		 * @brief Returns if a node is part of a \c LinkedList.
		 * @note This is done by checking if the pointer to the previous and next element is \c nullptr.
		 * @return \c false for is not in a \c LinkedList, otherwise \c true.
		 */
		bool isInAList()
		{
			if (m_previous != nullptr || m_next != nullptr)
				return true;
			else
				return false;
		}

	private:
		/** Pointer to the next element in a list.*/
		T* m_next = nullptr;
		/** Pointer to the previous element in a List.*/
		T* m_previous = nullptr;
	};

	/**
	 * @brief Implementation of a bidirectional iterator for \c LinkedList.
	 */
	class Iterator
	{
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using refernce = T&;

		Iterator() = default;
		/**
		 * @brief Constructor with member variable initialization.
		 * @param element Pointer to the element starting iterating.
		 */
		explicit Iterator(T* element)
		: m_node(element)
		{
		}
		virtual ~Iterator() = default;

		/**
		 * @brief Returns the reference of the element the iterator's position.
		 * @attention Using the reference for an iterator being nullptr
		 * (e.g. \c LinkedList::end) leads to undefined behavior.
		 * @return Reference to the element.
		 */
		T& operator*() const
		{
			return *m_node;
		}
		/**
		 * @brief Returns the pointer into the element the iterator's position.
		 * @attention Using the reference for an iterator being nullptr
		 * (e.g. \c LinkedList::end) leads to undefined behavior.
		 * @return Pointer into the element.
		 */
		T* operator->() const
		{
			return m_node;
		}
		/**
		 * @brief Iterates to the next element in the list.
		 * @attention Using the reference for an iterator being nullptr
		 * (e.g. \c LinkedList::end) leads to undefined behavior.
		 * @return Next element in the list.
		 */
		Iterator& operator++()
		{
			m_node = m_node->LinkedList::Node::next();
			return *this;
		}
		/**
		 * @brief Iterates to the next element in the list.
		 * @attention Using the reference for an iterator being nullptr
		 * (e.g. \c LinkedList::end) leads to undefined behavior.
		 * @return Copy of unchanged \c this.
		 */
		Iterator operator++(int)
		{
			Iterator temp = *this;
			m_node = m_node->LinkedList::Node::next();
			return temp;
		}
		/**
		 * @brief Iterates to the previous element in the list.
		 * @attention Using the reference for an iterator being nullptr
		 * (e.g. \c LinkedList::end) leads to undefined behavior.
		 * @return Previous element in the list.
		 */
		Iterator& operator--()
		{
			m_node = m_node->LinkedList::Node::previous();
			return *this;
		}
		/**
		 * @brief Iterates to the previous element in the list.
		 * @attention Using the reference for an iterator being nullptr
		 * (e.g. \c LinkedList::end) leads to undefined behavior.
		 * @return Copy of unchanged \c this.
		 */
		Iterator operator--(int)
		{
			Iterator temp = *this;
			temp.m_node = temp.m_node->LinkedList::Node::previous();
			return temp;
		}
		/**
		 * @brief Compares \c this element with \c that element.
		 * @param that Iterator to compare.
		 * @return \c true for both iterator's position is the same element;
		 * \c false for both iterator's position are different elements.
		 */
		bool operator==(const Iterator& that) const
		{
			return m_node == that.m_node;
		}
		/**
		 * @brief Compares \c this element with \c that element.
		 * @param that Iterator to compare.
		 * @return \c true for both iterators are different elements;
		 * \c false for both iterators are the same element.
		 */
		bool operator!=(const Iterator& that) const
		{
			return m_node != that.m_node;
		}

	private:
		/**Pointer to the node organizing*/
		T* m_node = nullptr;
	};

	/**
	 * @brief Implementation of a bidirectional constant iterator for \c LinkedList.
	 */
	class ConstIterator
	{
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using refernce = T&;

		ConstIterator() = default;
		/**
		 * @brief Constructor with member variable initialization.
		 * @param element Pointer to the element starting iterating.
		 */
		explicit ConstIterator(const T* element)
		: m_node(element)
		{
		}
		/**
		 * @brief Copy constructor.
		 * @param iterator iterator to copy from.
		 */
		explicit ConstIterator(const Iterator& iterator)
		: m_node(iterator.m_node)
		{
		}
		virtual ~ConstIterator() = default;

		/**
		 * @brief Returns the reference of the element the iterator's position.
		 * @attention Using the reference for an iterator being nullptr
		 * (e.g. \c LinkedList::end) leads to undefined behavior.
		 * @return Reference to the element.
		 */
		const T& operator*() const
		{
			return *m_node;
		}
		/**
		 * @brief Returns the pointer into the element the iterator's position.
		 * @attention Using the reference for an iterator being nullptr
		 * (e.g. \c LinkedList::end) leads to undefined behavior.
		 * @return Pointer into the element.
		 */
		const T* operator->() const
		{
			return m_node;
		}
		/**
		 * @brief Iterates to the next element in the list.
		 * @attention Using the reference for an iterator being nullptr
		 * (e.g. \c LinkedList::end) leads to undefined behavior.
		 * @return Next element in the list.
		 */
		ConstIterator& operator++()
		{
			m_node = m_node->LinkedList::Node::next();
			return *this;
		}
		/**
		 * @brief Iterates to the next element in the list.
		 * @attention Using the reference for an iterator being nullptr
		 * (e.g. \c LinkedList::end) leads to undefined behavior.
		 * @return Copy of unchanged \c this.
		 */
		ConstIterator operator++(int)
		{
			ConstIterator temp = *this;
			m_node = m_node->LinkedList::Node::next();
			return temp;
		}
		/**
		 * @brief Iterates to the previous element in the list.
		 * @attention Using the reference for an iterator being nullptr
		 * (e.g. \c LinkedList::end) leads to undefined behavior.
		 * @return Previous element in the list.
		 */
		ConstIterator& operator--()
		{
			m_node = m_node->LinkedList::Node::previous();
			return *this;
		}
		/**
		 * @brief Iterates to the previous element in the list.
		 * @attention Using the reference for an iterator being nullptr
		 * (e.g. \c LinkedList::end) leads to undefined behavior.
		 * @return Copy of unchanged \c this.
		 */
		ConstIterator operator--(int)
		{
			ConstIterator temp = *this;
			m_node = m_node->LinkedList::Node::previous();
			return temp;
		}
		/**
		 * @brief Compares \c this element with \c that element.
		 * @param other Iterator to compare.
		 * @return \c true for both iterator's position is the same element;
		 * \c false for both iterator's position are different elements.
		 */
		bool operator==(const ConstIterator& other) const
		{
			return m_node == other.m_node;
		}
		/**
		 * @brief Compares \c this element with \c that element.
		 * @param other Iterator to compare.
		 * @return \c true for both iterators are different elements;
		 * \c false for both iterators are the same element.
		 */
		bool operator!=(const ConstIterator& other) const
		{
			return m_node != other.m_node;
		}

	private:
		/**Pointer to the node organizing*/
		const T* m_node = nullptr;
	};
	/**
	 * @brief Creates an empty List
	 */
	LinkedList();
	/**
	 * @brief Returns a reference to the first element in the list.
	 *
	 * Unlike member \c LinkedList::begin, which returns an iterator to this same element,
	 * this function returns a direct reference.
	 * Calling this function on a not existing element causes undefined behavior.
	 * @return Reference to the first element.
	 */
	T& front();
	/**
	 * @brief Returns a reference to the first element in the list.
	 *
	 * Unlike member \c LinkedList::begin, which returns an iterator to this same element,
	 * this function returns a direct reference.
	 * Calling this function on a not existing element causes undefined behavior.
	 * @return For a const-qualified list object returns a const reference to the first element.
	 */
	const T& front() const;
	/**
	 * @brief Returns a reference to the last element in the list.
	 *
	 * Unlike member \c LinkedList::end, which returns an iterator to this same element,
	 * this function returns a direct reference.
	 * Calling this function on a not existing element causes undefined behavior.
	 * @return Reference to the last element.
	 */
	T& back();
	/**
	 * @brief Returns a reference to the last element in the list.
	 *
	 * Unlike member \c LinkedList::end, which returns an iterator to this same element,
	 * this function returns a direct reference.
	 * Calling this function on a not existing element causes undefined behavior.
	 * @return For a const-qualified list object returns a const reference to the last element.
	 */
	const T& back() const;
	/**
	 * @brief Returns an iterator pointing to the first element in the list.
	 * @attention Notice that, unlike member LinkedList::front, which returns a reference to the first element,
	 * this function returns a bidirectional iterator pointing to it.
	 * If the list is empty, the returned iterator value shall not be dereferenced.
	 * @return An iterator to the beginning of the sequence.
	 */
	Iterator begin();
	/**
	 * @brief Returns an iterator pointing to the first element in the list.
	 * @attention Notice that, unlike member LinkedList::front, which returns a reference to the first element,
	 * this function returns a bidirectional iterator pointing to it.
	 * If the list is empty, the returned iterator value shall not be dereferenced.
	 * @return An iterator to the beginning of the sequence.
	 */
	ConstIterator begin() const;
	/**
	 * @brief Returns an iterator pointing to the first element in the list.
	 * @attention Notice that, unlike member LinkedList::front, which returns a reference to the first element,
	 * this function returns a bidirectional iterator pointing to it.
	 * If the list is empty, the returned iterator value shall not be dereferenced.
	 * @return An iterator to the beginning of the sequence.
	 */
	ConstIterator cbegin() const;
	/**
	 * @brief Returns an iterator referring to the past-the-end element in the list.
	 *
	 * The past-the-end element is the theoretical element that would follow the last
	 * element in the list. It does not point to any element, and thus shall not be dereferenced.
	 * This function is often used in combination with LinkedList::begin to specify a range including
	 * all the elements in the range.
	 * If LinkedList is empty, this function returns the same as LinkedList::begin.
	 * @return An iterator to the element past the end of the sequence.
	 */
	Iterator end();
	/**
	 * @brief Returns an iterator referring to the past-the-end element in the list.
	 *
	 * The past-the-end element is the theoretical element that would follow the last
	 * element in the list. It does not point to any element, and thus shall not be dereferenced.
	 * This function is often used in combination with LinkedList::begin to specify a range including
	 * all the elements in the range.
	 * If LinkedList is empty, this function returns the same as LinkedList::begin.
	 * @return An iterator to the element past the end of the sequence.
	 */
	ConstIterator end() const;
	/**
	 * @brief Returns an iterator referring to the past-the-end element in the list.
	 *
	 * The past-the-end element is the theoretical element that would follow the last
	 * element in the list. It does not point to any element, and thus shall not be dereferenced.
	 * This function is often used in combination with LinkedList::begin to specify a range including
	 * all the elements in the range.
	 * If LinkedList is empty, this function returns the same as LinkedList::begin.
	 * @return An iterator to the element past the end of the sequence.
	 */
	ConstIterator cend() const;
	/**
	 * @brief Returns whether the list is empty (i.e. whether its size is 0).
	 *
	 * This function does not modify the list in any way.
	 * To clear the content of a list container, see LinkedList::clear.
	 * @return \c true if the list size is 0, \c false otherwise.
	 */
	bool empty() const;
	/**
	 * @brief Returns the number of elements in the list.
	 * @return Number of elements in the list.
	 */
	size_t size() const;
	/**
	 * @brief Removes all elements from the list and
	 * leaving the list with a size of 0.
	 */
	void clear();
	/**
	 * @brief The list is extended by inserting new elements before the element at the specified position.
	 * @param position Position in the list where the new element is inserted.
	 * @param element Element to be inserted into the list.
	 * @return An iterator that points to the newly inserted element.
	 */
	Iterator insert(Iterator position, T& element);
	/**
	 * @brief Removes a single element from the list.
	 *
	 * This effectively reduces the list size by one.
	 * @attention This function does not check whether the given range points to \c this.
	 * @param position Position to erase from the list. Position has to have a exiting element.
	 * @return An iterator pointing to the element that followed the node erased by the function call.
	 */
	Iterator erase(Iterator position);
	/**
	 * @brief Removes multiple elements from the list.
	 *
	 * This effectively reduces the list size by the number if erased elements.
	 * @attention This function does not check whether \c position points to an element inside \c this.
	 * @param first First element of a sequence to remove from the list.
	 * @param last Last element of a sequence to remove from the list.
	 * @return An iterator pointing to the element that followed the last element
	 * erased by the function call.
	 */
	Iterator erase(Iterator first, Iterator last);
	/**
	 * @brief Adds a new element to the end of the list, after its current last element.
	 *
	 * The content of element is moved into the list.
	 * This effectively increases the list size by one.
	 * @param element Element to add to the end of the list.
	 */
	void pushBack(T& element);
	/**
	 * @brief Adds a new element to the beginning of the list,
	 * before its current first element.
	 *
	 * The content of element is moved into the list.
	 * This effectively increases the list size by one.
	 * @param element Element to add to the beginning of the list.
	 */
	void pushFront(T& element);
	/**
	 * @brief Removes the last element in the list,
	 * effectively reducing the list size by one.
	 */
	void popBack();
	/**
	 * @brief Removes the first element in the list,
	 * effectively reducing the list size by one.
	 */
	void popFront();
	/**
	 * @brief Assigns new contents to the list,
	 * replacing its current contents, and modifying its size accordingly.
	 *
	 * @param list A list object of the same type (i.e., with the same template parameter).
	 * @return Reference to this list.
	 */
	LinkedList<T>& operator=(const LinkedList<T>& list);

private:
	/**
	 * @brief Puts an element into an empty list.
	 * @param element Element to push.
	 */
	void pushEmpty(T& element);
	/**Pointer to the fist element in the list*/
	T* m_front = nullptr;
	/**Element behind the last element in the list*/
	Node m_end;
	/**Count of elements in the list*/
	size_t m_size = 0;
};

template <class T>
LinkedList<T>::LinkedList()
{
	clear();
}

template <class T>
T& LinkedList<T>::front()
{
	return *m_front;
}

template <class T>
const T& LinkedList<T>::front() const
{
	return *m_front;
}

template <class T>
T& LinkedList<T>::back()
{
	return *m_end.previous();
}

template <class T>
const T& LinkedList<T>::back() const
{
	return *m_end.previous();
}

template <class T>
typename LinkedList<T>::Iterator LinkedList<T>::begin()
{
	return m_front ? Iterator(m_front) : end();
}

template <class T>
typename LinkedList<T>::ConstIterator LinkedList<T>::begin() const
{
	return m_front ? ConstIterator(m_front) : end();
}

template <class T>
typename LinkedList<T>::ConstIterator LinkedList<T>::cbegin() const
{
	return m_front ? ConstIterator(m_front) : cend();
}

template <class T>
typename LinkedList<T>::Iterator LinkedList<T>::end()
{
	return Iterator(reinterpret_cast<T*>(&m_end));
}

template <class T>
typename LinkedList<T>::ConstIterator LinkedList<T>::end() const
{
	return ConstIterator(reinterpret_cast<const T*>(&m_end));
}

template <class T>
typename LinkedList<T>::ConstIterator LinkedList<T>::cend() const
{
	return ConstIterator(reinterpret_cast<const T*>(&m_end));
}

template <class T>
bool LinkedList<T>::empty() const
{
	return m_size == 0;
}

template <class T>
size_t LinkedList<T>::size() const
{
	return m_size;
}

template <class T>
void LinkedList<T>::clear()
{
	m_front = nullptr;
	m_end.setPrevious(m_front);
	m_size = 0;
}

template <class T>
typename LinkedList<T>::Iterator LinkedList<T>::insert(Iterator position, T& element)
{
	Iterator ret = position;
	if (&(*position) == m_front)
	{
		pushFront(element);
		ret = Iterator(m_front);
	}
	else if (m_size == 0)
	{
		m_front = &element;
		m_end.setPrevious(&element);
		m_size++;
		ret = Iterator(m_front);
	}
	else
	{
		T* nex = &(*position);
		nex->LinkedList::Node::setPrevious(&element);
		T* pre = &(*position--);
		pre->LinkedList::Node::setNext(&element);
	}
	return ret;
}

template <class T>
typename LinkedList<T>::Iterator LinkedList<T>::erase(Iterator position)
{
	if (&(*position) == m_front)
	{
		popFront();
		return begin();
	}
	else if (&(*position) == m_end.previous())
	{
		popBack();
		return end();
	}
	else if (m_size > 0)
	{
		T* pre = (&(*position))->LinkedList::Node::previous();
		T* nex = (&(*position))->LinkedList::Node::next();
		pre->LinkedList::Node::setNext(nex);
		nex->LinkedList::Node::setPrevious(pre);
		m_size--;
		return ++position;
	}
	return Iterator();
}

template <class T>
typename LinkedList<T>::Iterator LinkedList<T>::erase(Iterator first, Iterator last)
{
	while (first != last)
	{
		first = erase(first);
	}
	return erase(first);
}

template <class T>
void LinkedList<T>::pushBack(T& element)
{
	element.LinkedList::Node::setNext(nullptr);

	if (m_size >= 1)
	{
		back().LinkedList::Node::setNext(&element);
		element.LinkedList::Node::setPrevious(m_end.previous());
		element.LinkedList::Node::setNext(reinterpret_cast<T*>(&m_end));
		m_end.setPrevious(&element);
	}
	else  // m_size == 0
	{
		pushEmpty(element);
	}
	m_size++;
}

template <class T>
void LinkedList<T>::pushFront(T& element)
{
	element.LinkedList::Node::setPrevious(nullptr);
	if (m_size >= 1)
	{
		element.LinkedList::Node::setNext(m_front);
		m_front->LinkedList::Node::setPrevious(element);
		m_front = &element;
	}
	else  // m_size == 0
	{
		pushEmpty(element);
	}
	m_size++;
}

template <class T>
void LinkedList<T>::popBack()
{
	if (m_size == 1)
	{
		clear();
	}
	else if (m_size > 1)
	{
		Iterator newPrev = (end()--)--;
		m_end.setPrevious(&(*newPrev));
		newPrev->LinkedList::Node::setNext(reinterpret_cast<T*>(&m_end));
		m_size--;
	}
}

template <class T>
void LinkedList<T>::popFront()
{
	if (m_size == 1)
	{
		clear();
	}
	else
	{
		m_front = m_front->LinkedList::Node::next();
		m_size--;
	}
}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& list)
{
	this->m_front = list.m_front;
	this->m_end = list.m_end;
	this->m_size = list.m_size;
	return *this;
}
template <class T>
void semf::LinkedList<T>::pushEmpty(T& element)
{
	element.LinkedList::Node::setPrevious(nullptr);
	element.LinkedList::Node::setNext(reinterpret_cast<T*>(&m_end));
	m_end.setPrevious(&element);
	m_front = &element;
}
} /* namespace semf */
#endif /* SEMF_UTILS_CORE_LISTS_LINKEDLIST_H_ */
