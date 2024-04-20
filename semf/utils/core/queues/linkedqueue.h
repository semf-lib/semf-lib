/**
 * @file linkedqueue.h
 * @date 27.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_UTILS_CORE_QUEUES_LINKEDQUEUE_H_
#define SEMF_UTILS_CORE_QUEUES_LINKEDQUEUE_H_

#include <iterator>
#include <cstdint>
#include <cstddef>
#include <algorithm>

namespace semf
{
/**
 * @brief LinkedQueue is an managed single linked queue implementation.
 *
 * It is similar to std::queue, but works without dynamic storage allocation.
 * Every element in the queue has to inherit from \c LinkedQueue::Node and
 * in case has the information of the \c next() node.
 *
 * The LinkedQueue class is a list that gives the programmer the functionality
 * of a queue - specifically, a FIFO (first-in, first-out) data organization.
 *
 * @attention A node is only allowed to be part of one queue. Putting a node into
 * a second queue changes the reference in the element. That causes to
 * undefined behavior by iterating through the queues.
 *
 * @tparam T class type inherits from \c LinkedQueue::Node
 */
template <class T>
class LinkedQueue
{
public:
	/**
	 * @brief Implements the \c next() functionality for every element in a queue.
	 *
	 * A class type using \c LinkedQueue has to inherit from this class.
	 */
	class Node
	{
	public:
		virtual ~Node() = default;

		/**
		 * @brief Returns a pointer to the next element in a queue.
		 * @attention The last element in a queue will return \c nullptr.
		 * @return The next element in a queue.
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
		 * @brief Returns if a node is part of this \c LinkedQueue.
		 * @note This is done by checking if the pointer to the next element is \c nullptr and
		 * if this node is the last element in \c queue.
		 * @param queue Queue to check.
		 * @return \c false for is not in this \c LinkedQueue, otherwise \c true.
		 */
		bool isInQueue(LinkedQueue& queue)
		{
			Node* me = this;
			return queue.end() != std::find_if(queue.begin(), queue.end(), [&me](T& node) { return &node == me; });
		}

	private:
		/** Pointer to the next element in a queue.*/
		T* m_next = nullptr;
	};

	/**
	 * @brief Implementation of a forward iterator for \c LinkedQueue.
	 */
	class Iterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
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
		 * (e.g. \c LinkedQueue::end) leads to undefined behavior.
		 * @return Reference to the element.
		 */
		T& operator*() const
		{
			return *m_node;
		}
		/**
		 * @brief Returns the pointer into the element the iterator's position.
		 * @attention Using the reference for an iterator being nullptr
		 * (e.g. \c LinkedQueue::end) leads to undefined behavior.
		 * @return Pointer into the element.
		 */
		T* operator->() const
		{
			return m_node;
		}
		/**
		 * @brief Iterates to the next element in the list.
		 * @attention Using the reference for an iterator being nullptr
		 * (e.g. \c LinkedQueue::end) leads to undefined behavior.
		 * @return Next element in the queue.
		 */
		Iterator& operator++()
		{
			m_node = m_node->LinkedQueue::Node::next();
			return *this;
		}
		/**
		 * @brief Iterates to the next element in the queue.
		 * @attention Using the reference for an iterator being nullptr
		 * (e.g. \c LinkedQueue::end) leads to undefined behavior.
		 * @return Next element in the queue.
		 */
		Iterator operator++(int)
		{
			Iterator temp = *this;
			m_node = m_node->LinkedQueue::Node::next();
			return temp;
		}
		/**
		 * @brief Compares \c this element with \c that element.
		 * @param other Iterator to compare.
		 * @return \c true for both iterator's position is the same element;
		 * \c false for both iterator's position are different elements.
		 */
		bool operator==(const Iterator& other) const
		{
			return m_node == other.m_node;
		}
		/**
		 * @brief Compares \c this element with \c that element.
		 * @param other Iterator to compare.
		 * @return \c true for both iterators are different elements;
		 * \c false for both iterators are the same element.
		 */
		bool operator!=(const Iterator& other) const
		{
			return m_node != other.m_node;
		}

	private:
		/**Pointer to the node organizing*/
		T* m_node = nullptr;
	};

	/**
	 * @brief Implementation of a forward constant iterator for \c LinkedQueue.
	 */
	class ConstIterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
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
		 * (e.g. \c LinkedQueue::end) leads to undefined behavior.
		 * @return Reference to the element.
		 */
		const T& operator*() const
		{
			return *m_node;
		}
		/**
		 * @brief Returns the pointer into the element the iterator's position.
		 * @attention Using the reference for an iterator being nullptr
		 * (e.g. \c LinkedQueue::end) leads to undefined behavior.
		 * @return Pointer into the element.
		 */
		const T* operator->() const
		{
			return m_node;
		}
		/**
		 * @brief Iterates to the next element in the queue.
		 * @attention Using the reference for an iterator being nullptr
		 * (e.g. \c LinkedQueue::end) leads to undefined behavior.
		 * @return Next element in the queue.
		 */
		ConstIterator& operator++()
		{
			m_node = m_node->LinkedQueue::Node::next();
			return *this;
		}
		/**
		 * @brief Iterates to the next element in the queue.
		 * @attention Using the reference for an iterator being nullptr
		 * (e.g. \c LinkedQueue::end) leads to undefined behavior.
		 * @return Next element in the queue.
		 */
		ConstIterator operator++(int)
		{
			ConstIterator temp = *this;
			m_node = m_node->LinkedQueue::Node::next();
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
	 * @brief Returns a reference to the first element in the queue.
	 *
	 * This element will be the first element to be removed on a call to \c pop().
	 * Calling this function on a not existing element causes undefined behavior.
	 * @return Reference to the first element.
	 */
	T& front();
	/**
	 * @brief Returns a reference to the first element in the queue.
	 *
	 * This element will be the first element to be removed on a call to \c pop().
	 * Calling this function on a not existing element causes undefined behavior.
	 * @return For a const-qualified queue object returns a const reference to the first element.
	 */
	const T& front() const;
	/**
	 * @brief Returns a reference to the last element in the queue.
	 *
	 * This is the most recently pushed element.
	 * Calling this function on a not existing element causes undefined behavior.
	 * @return Reference to the last element.
	 */
	T& back();
	/**
	 * @brief Returns a reference to the last element in the queue.
	 *
	 * This is the most recently pushed element.
	 * Calling this function on a not existing element causes undefined behavior.
	 * @return For a const-qualified queue object returns a const reference to the last element.
	 */
	const T& back() const;
	/**
	 * @brief Returns an iterator pointing to the first element in the queue.
	 * @attention Notice that, unlike member LinkedQueue::front, which returns a reference to the first element,
	 * this function returns a forward iterator pointing to it.
	 * If the queue is empty, the returned iterator value shall not be dereferenced.
	 * @return An iterator to the beginning of the sequence.
	 */
	Iterator begin();
	/**
	 * @brief Returns an iterator pointing to the first element in the queue.
	 * @attention Notice that, unlike member LinkedQueue::front, which returns a reference to the first element,
	 * this function returns a forward iterator pointing to it.
	 * If the queue is empty, the returned iterator value shall not be dereferenced.
	 * @return An iterator to the beginning of the sequence.
	 */
	ConstIterator begin() const;
	/**
	 * @brief Returns an iterator pointing to the first element in the queue.
	 * @attention Notice that, unlike member LinkedQueue::front, which returns a reference to the first element,
	 * this function returns a forward iterator pointing to it.
	 * If the queue is empty, the returned iterator value shall not be dereferenced.
	 * @return An iterator to the beginning of the sequence.
	 */
	ConstIterator cbegin() const;
	/**
	 * @brief Returns an iterator referring to the past-the-end element in the queue.
	 *
	 * The past-the-end element is the theoretical element that would follow the last
	 * element in the queue. It does not point to any element, and thus shall not be dereferenced.
	 * This function is often used in combination with LinkedQueue::begin to specify a range including
	 * all the elements in the range.
	 * If LinkedQueue is empty, this function returns the same as LinkedQueue::begin.
	 * @return An iterator to the element past the end of the sequence.
	 */
	Iterator end();
	/**
	 * @brief Returns an iterator referring to the past-the-end element in the queue.
	 *
	 * The past-the-end element is the theoretical element that would follow the last
	 * element in the queue. It does not point to any element, and thus shall not be dereferenced.
	 * This function is often used in combination with LinkedQueue::begin to specify a range including
	 * all the elements in the range.
	 * If LinkedQueue is empty, this function returns the same as LinkedQueue::begin.
	 * @return An iterator to the element past the end of the sequence.
	 */
	ConstIterator end() const;
	/**
	 * @brief Returns an iterator referring to the past-the-end element in the queue.
	 *
	 * The past-the-end element is the theoretical element that would follow the last
	 * element in the queue. It does not point to any element, and thus shall not be dereferenced.
	 * This function is often used in combination with LinkedQueue::begin to specify a range including
	 * all the elements in the range.
	 * If LinkedQueue is empty, this function returns the same as LinkedQueue::begin.
	 * @return An iterator to the element past the end of the sequence.
	 */
	ConstIterator cend() const;
	/**
	 * @brief Returns whether the queue is empty (i.e. whether its size is 0).
	 *
	 * This function does not modify the queue in any way.
	 * @return \c true if the queue size is 0, \c false otherwise.
	 */
	bool empty() const;
	/**
	 * @brief Returns the number of elements in the queue.
	 * @return Number of elements in the queue.
	 */
	size_t size() const;
	/**
	 * @brief Adds a new element to the end of the queue, after its current last element.
	 *
	 * The content of element is moved into the queue.
	 * This effectively increases the queue size by one.
	 * @param element Element to add to the end of the queue.
	 */
	void push(T& element);
	/**
	 * @brief Removes the first element in the queue,
	 * effectively reducing the queue size by one.
	 */
	void pop();
	/**
	 * @brief Assigns new contents to the queue,
	 * replacing its current contents, and modifying its size accordingly.
	 *
	 * @param queue A queue object of the same type (i.e., with the same template parameter).
	 * @return Reference to this queue.
	 */
	LinkedQueue<T>& operator=(const LinkedQueue<T>& queue);

private:
	/**Pointer to the fist element in the queue*/
	T* m_front = nullptr;
	/**Pointer to the last element in the queue*/
	T* m_back = nullptr;
	/**Count of elements in the queue*/
	size_t m_size = 0;
};

template <class T>
T& LinkedQueue<T>::front()
{
	return *m_front;
}

template <class T>
const T& LinkedQueue<T>::front() const
{
	return *m_front;
}

template <class T>
T& LinkedQueue<T>::back()
{
	return *m_back;
}

template <class T>
const T& LinkedQueue<T>::back() const
{
	return *m_back;
}

template <class T>
typename LinkedQueue<T>::Iterator LinkedQueue<T>::begin()
{
	return Iterator(m_front);
}

template <class T>
typename LinkedQueue<T>::ConstIterator LinkedQueue<T>::begin() const
{
	return ConstIterator(m_front);
}

template <class T>
typename LinkedQueue<T>::ConstIterator LinkedQueue<T>::cbegin() const
{
	return ConstIterator(m_front);
}

template <class T>
typename LinkedQueue<T>::Iterator LinkedQueue<T>::end()
{
	return Iterator(nullptr);
}

template <class T>
typename LinkedQueue<T>::ConstIterator LinkedQueue<T>::end() const
{
	return ConstIterator(nullptr);
}

template <class T>
typename LinkedQueue<T>::ConstIterator LinkedQueue<T>::cend() const
{
	return ConstIterator(nullptr);
}

template <class T>
bool LinkedQueue<T>::empty() const
{
	return m_size == 0;
}

template <class T>
size_t LinkedQueue<T>::size() const
{
	return m_size;
}

template <class T>
void LinkedQueue<T>::push(T& element)
{
	element.LinkedQueue::Node::setNext(nullptr);
	if (m_size > 1)
	{
		m_back->LinkedQueue::Node::setNext(&element);
		m_back = &element;
	}
	else if (m_size == 1)
	{
		m_front->LinkedQueue::Node::setNext(&element);
		m_back = &element;
	}
	else  // m_size == 0
	{
		m_front = &element;
		m_back = &element;
	}
	m_size++;
}

template <class T>
void LinkedQueue<T>::pop()
{
	if (m_size == 1)
	{
		m_front = nullptr;
		m_back = nullptr;
		m_size--;
	}
	else
	{
		m_front = m_front->LinkedQueue::Node::next();
		m_size--;
	}
}

template <class T>
LinkedQueue<T>& LinkedQueue<T>::operator=(const LinkedQueue<T>& queue)
{
	this->m_front = queue.m_front;
	this->m_back = queue.m_back;
	this->m_size = queue.m_size;
	return *this;
}
} /* namespace semf */
#endif /* SEMF_UTILS_CORE_QUEUES_LINKEDQUEUE_H_ */
