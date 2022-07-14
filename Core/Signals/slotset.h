/**
 * @file slotset.h
 * 
 * @date 08.09.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory
 */

#ifndef CORE_SIGNALS_SLOTSET_H_
#define CORE_SIGNALS_SLOTSET_H_

#include <Core/Signals/slot.h>
#include <cstddef>

namespace semf
{
/**
 * @brief Handles \c N_SLOTS slots.
 * @tparam N_SLOTS Number of slots.
 * @tparam Arguments Argument types of the slots.
 */
template<size_t N_SLOTS, typename... Arguments>
class SlotSet
{
public:
	/**Simple iterator typedef.*/
	using iterator = Slot<Arguments...>*;
	/**
	 * @brief Inserts \c slot into the \c SlotSet. If the \c SlotSet is full or a slot equal to \c slot is already inside the \c SlotSet nothing happens.
	 * @param slot New slot to add to the \c SlotSet.
	 */
	void insert(Slot<Arguments...>&& slot);
	/**
	 * @brief Removes \c slot from the \c SlotSet.
	 * @param slot Slot to be removed.
	 */
	void erase(Slot<Arguments...>&& slot);
	/**
	 * @brief Removes all slots from the \c SlotSet.
	 */
	void clear();
	/**
	 * @brief Checks whether the \c SlotSet is empty or not.
	 * @return \c true if empty, \c false otherwise.
	 */
	bool empty() const;
	/**
	 * @brief Begin iterator of the \c SlotSet.
	 * @return \c iterator pointing to the first slot.
	 */
	iterator begin() const;
	/**
	 * @brief End iterator of the \c SlotSet.
	 * @return \c iterator pointing to element behind the last slot.
	 */
	iterator end() const;
private:
	/**Array of slots.*/
	Slot<Arguments...> m_slots[N_SLOTS];
	/**Begin iterator.*/
	iterator m_begin = m_slots;
	/**End iterator.*/
	iterator m_end = m_slots;
};

template<size_t N_SLOTS, typename... Arguments>
void SlotSet<N_SLOTS, Arguments...>::insert(Slot<Arguments...>&& slot)
{
	for (iterator it = m_slots; it != m_end; it++)
	{
		if (slot == *it)
			return;
	}

	if (m_end != m_slots + N_SLOTS)
	{
		*m_end = slot;
		m_end++;
	}
}

template<size_t N_SLOTS, typename... Arguments>
void SlotSet<N_SLOTS, Arguments...>::erase(Slot<Arguments...>&& slot)
{
	for (iterator it = m_slots; it != m_end; it++)
	{
		if (*it == slot)
		{
			for (iterator next = it + 1; next != m_end; next++, it++)
			{
				*it = *next;
			}
			m_end--;
			return;
		}
	}
}

template<size_t N_SLOTS, typename... Arguments>
void SlotSet<N_SLOTS, Arguments...>::clear()
{
	m_end = m_slots;
}

template<size_t N_SLOTS, typename... Arguments>
bool SlotSet<N_SLOTS, Arguments...>::empty() const
{
	return m_end == m_slots;
}

template<size_t N_SLOTS, typename... Arguments>
typename SlotSet<N_SLOTS, Arguments...>::iterator SlotSet<N_SLOTS, Arguments...>::begin() const
{
	return m_begin;
}

template<size_t N_SLOTS, typename... Arguments>
typename SlotSet<N_SLOTS, Arguments...>::iterator SlotSet<N_SLOTS, Arguments...>::end() const
{
	return m_end;
}

}  // namespace semf
#endif  // CORE_SIGNALS_SLOTSET_H_
