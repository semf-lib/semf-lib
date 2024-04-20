/**
 * @file signal.h
 *
 * @date 11.05.2023
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory
 */

#ifndef SEMF_UTILS_CORE_SIGNALS_SIGNAL_H_
#define SEMF_UTILS_CORE_SIGNALS_SIGNAL_H_

#include <semf/utils/core/signals/slotbase.h>
#include <cstddef>
#include <utility>

/*!
	@brief Creates a semf signal.
	@param name	Name of the created signal.
	@param ... Additional parameter types.
*/
#define SEMF_SIGNAL(name, ...) semf::Signal<__VA_ARGS__> name

namespace semf
{
/**
 * @brief Signal for lightweight signal/slot implementation. One signal can be connected to multiple slots and one slot can be connected to multiple signal.
 *        Depending on the type of callback function you may want to use you need to connect using different implementations of \c SlotBase.
 * @note
 * <ul>
 * <li>Its easy possible to make 1:n and n:1 connections.</li>
 * <li>m:n connections are possible, with the following limitation:
 * A slot can no longer be connected to a signal if it has been connected to a signal that was previously connected to another slot.</li>
 * </ul>
 *
 * @tparam Arguments The parameter types for pass parameter over the signal to a function or method.
 */
template <typename... Arguments>
class Signal
{
public:
	/**
	 * @brief Disconnect all functions or methods
	 */
	void clear();
	/**
	 * @brief Connect a method to the signal.
	 * @param slot The slot which should connect.
	 */
	void connect(SlotBase<Arguments...>& slot);
	/**
	 * @brief Disonnect a method from the signal.
	 * @param slot The slot which should disconnect.
	 */
	void disconnect(SlotBase<Arguments...>& slot);
	/**
	 * @brief Call the connected methods or functions.
	 * @param arguments The arguments which should pass to the methods or functions.
	 */
	void emitSignal(Arguments... arguments) const;
	/**
	 * @brief Call the connected methods or functions.
	 * @param arguments The arguments which should pass to the methods or functions.
	 */
	void operator()(Arguments... arguments) const;
	/**
	 * @brief Checks if the \c Signal is connected to any slot.
	 * @return \c true if no connection exists, \c false otherwise.
	 */
	bool empty() const;

private:
	/**Slot for handling the callback invocation.*/
	SlotBase<Arguments...>* m_slot = nullptr;
	/**Number of slots connected to the signal.*/
	size_t m_connectedSlots = 0;
};

template <typename... Arguments>
void Signal<Arguments...>::clear()
{
	SlotBase<Arguments...>** nextSlot;
	for (auto** i = &m_slot; m_connectedSlots && *i; i = nextSlot)
	{
		nextSlot = &((*i)->m_nextSlot);
		*i = nullptr;

		m_connectedSlots--;
	}
}

template <typename... Arguments>
void Signal<Arguments...>::connect(SlotBase<Arguments...>& slot)
{
	/* is slot already in use? */
	if (slot.m_nextSlot)
		return;

	slot.m_nextSlot = m_slot;
	m_slot = &slot;

	m_connectedSlots++;
}

template <typename... Arguments>
void Signal<Arguments...>::disconnect(SlotBase<Arguments...>& slot)
{
	auto** i = &m_slot;
	for (; *i && (*i) != &slot; i = &((*i)->m_nextSlot))
		continue;

	if (*i)
	{
		(*i) = (*i)->m_nextSlot;
		slot.m_nextSlot = nullptr;
		m_connectedSlots--;
	}
}

template <typename... Arguments>
void Signal<Arguments...>::emitSignal(Arguments... arguments) const
{
	auto slot = m_slot;
	for (size_t i = 0; i < m_connectedSlots; i++)
	{
		slot->call(std::forward<Arguments>(arguments)...);
		slot = slot->m_nextSlot;
	}
}

template <typename... Arguments>
void Signal<Arguments...>::operator()(Arguments... arguments) const
{
	auto slot = m_slot;
	for (size_t i = 0; i < m_connectedSlots; i++)
	{
		slot->call(std::forward<Arguments>(arguments)...);
		slot = slot->m_nextSlot;
	}
}

template <typename... Arguments>
bool Signal<Arguments...>::empty() const
{
	return !m_slot;
}

}  // namespace semf
#endif  // SEMF_UTILS_CORE_SIGNALS_SIGNAL_H_
