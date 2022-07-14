/**
 * @file signal.h
 * 
 * @date 08.09.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory
 */

#ifndef CORE_SIGNALS_SIGNAL_H_
#define CORE_SIGNALS_SIGNAL_H_

#include <Core/Signals/slotset.h>
#include <cstddef>
#include <utility>

namespace semf
{
/**
 * @brief Signal for lightweight signal/slot implementation.
 * This Signal can connect with any method or function which have a
 * return value from type void.
 * @param N_SLOTS Describes how many methods or function can connect with this signal.
 * @param Arguments The parameter types for pass parameter over the signal to a function or method.
 */
template<size_t N_SLOTS = 1, typename... Arguments>
class Signal
{
public:
	/**
	 * @brief Disconnect all functions or methods
	 */
	void clear()
	{
		m_set.clear();
	}
	/**
	 * @brief Connect a method to the signal.
	 * @tparam X Method type.
	 * @tparam Y Object type.
	 * @param object The object which should connect.
	 * @param function The method which should connect.
	 */
	template<typename X, typename Y>
	void connect(Y* object, void (X::* function)(Arguments...))
	{
		m_set.insert(Slot<Arguments...>(object, function));
	}
	/**
	 * @brief Connect a constant method to the signal.
	 * @tparam X Method type.
	 * @tparam Y Object type.
	 * @param object The object which should connect.
	 * @param function The method which should connect.
	 */
	template<typename X, typename Y>
	void connect(Y* object, void (X::* function)(Arguments...) const)
	{
		m_set.insert(Slot<Arguments...>(object, function));
	}
	/**
	 * @brief Connect a function to the signal.
	 * @param function The function which should connect.
	 */
	void connect(void (*function)(Arguments...))
	{
		m_set.insert(Slot<Arguments...>(function));
	}
	/**
	 * @brief Disconnect a method from the signal.
	 * @tparam X Method type.
	 * @tparam Y Object type.
	 * @param object The object which should disconnect.
	 * @param function The method which should disconnect.
	 */
	template<typename X, typename Y>
	void disconnect(Y* object, void (X::* function)(Arguments...))
	{
		m_set.erase(Slot<Arguments...>(object, function));
	}
	/**
	 * @brief Disconnect a constant method from the signal.
	 * @tparam X Method type.
	 * @tparam Y Object type.
	 * @param object The object which should disconnect.
	 * @param function The method which should disconnect.
	 */
	template<typename X, typename Y>
	void disconnect(Y* object, void (X::* function)(Arguments...) const)
	{
		m_set.erase(Slot<Arguments...>(object, function));
	}
	/**
	 * @brief Disconnect a function from the signal
	 * @param function The function which should disconnect.
	 */
	void disconnect(void (*function)(Arguments...))
	{
		m_set.erase(Slot<Arguments...>(function));
	}
	/**
	 * @brief Call the connected methods or functions.
	 * @param arguments The arguments which should pass to the methods or functions.
	 */
	void emitSignal(Arguments... arguments) const
	{
		for (Slot<Arguments...>& slot : m_set)
		{
			slot.emitSlot(std::forward<Arguments>(arguments)...);
		}
	}
	/**
	 * @brief Call the connected methods or functions.
	 * @param arguments The arguments which should pass to the methods or functions.
	 */
	void operator() (Arguments... arguments) const
	{
		for (Slot<Arguments...>& slot : m_set)
		{
			slot.emitSlot(std::forward<Arguments>(arguments)...);
		}
	}
	/**
	 * @brief Checks if the \c Signal is connected to slot.
	 * @return \c true if no connection exists, \c false otherwise.
	 */
	bool empty() const
	{
		return !m_set.begin()->connected();
	}
private:
	/**\c SlotSet for storing all slots.*/
	SlotSet<N_SLOTS, Arguments...> m_set;
};

/**
 * @brief Signal for lightweight signal/slot implementation.
 * This Signal can connect with any method or function which have a
 * return value from type void.
 * @param Arguments The parameter types for pass parameter over the signal to a function or method.
 */
template<typename... Arguments>
class Signal<1, Arguments...>
{
public:
	/**
	 * @brief Disconnect all functions or methods
	 */
	void clear()
	{
		m_slot.disconnect();
	}
	/**
	 * @brief Connect a method to the signal.
	 * @tparam X Method type.
	 * @tparam Y Object type.
	 * @param object The object which should connect.
	 * @param function The method which should connect.
	 */
	template<typename X, typename Y>
	void connect(Y* object, void (X::* function)(Arguments...))
	{
		if (!m_slot.connected())
			m_slot = Slot<Arguments...>(object, function);
	}
	/**
	 * @brief Connect a constant method to the signal.
	 * @tparam X Method type.
	 * @tparam Y Object type.
	 * @param object The object which should connect.
	 * @param function The method which should connect.
	 */
	template<typename X, typename Y>
	void connect(Y* object, void (X::* function)(Arguments...) const)
	{
		if (!m_slot.connected())
			m_slot = Slot<Arguments...>(object, function);
	}
	/**
	 * @brief Connect a function to the signal.
	 * @param function The function which should connect.
	 */
	void connect(void (*function)(Arguments...))
	{
		if (!m_slot.connected())
			m_slot = Slot<Arguments...>(function);
	}
	/**
	 * @brief Overwrites the previous connected callback with a new one.
	 * @tparam X Method type.
	 * @tparam Y Object type.
	 * @param object The object which should connect.
	 * @param function The method which should connect.
	 */
	template<typename X, typename Y>
	void reconnect(Y* object, void (X::* function)(Arguments...))
	{
		m_slot = Slot<Arguments...>(object, function);
	}
	/**
	 * @brief Overwrites the previous connected callback with a new one.
	 * @tparam X Method type.
	 * @tparam Y Object type.
	 * @param object The object which should connect.
	 * @param function The method which should connect.
	 */
	template<typename X, typename Y>
	void reconnect(Y* object, void (X::* function)(Arguments...) const)
	{
		m_slot = Slot<Arguments...>(object, function);
	}
	/**
	 * @brief Overwrites the previous connected callback with a new one.
	 * @param function The function which should connect.
	 */
	void reconnect(void (*function)(Arguments...))
	{
		m_slot = Slot<Arguments...>(function);
	}
	/**
	 * @brief Disonnect a method from the signal.
	 * @tparam X Method type.
	 * @tparam Y Object type.
	 * @param object The object which should disconnect.
	 * @param function The method which should disconnect.
	 */
	template<typename X, typename Y>
	void disconnect(Y* object, void (X::* function)(Arguments...))
	{
		Slot<Arguments...> compareSlot(object, function);
		if (m_slot == compareSlot)
			m_slot.disconnect();
	}
	/**
	 * @brief Disconnect a constant method from the signal.
	 * @tparam X Method type.
	 * @tparam Y Object type.
	 * @param object The object which should disconnect.
	 * @param function The method which should disconnect.
	 */
	template<typename X, typename Y>
	void disconnect(Y* object, void (X::* function)(Arguments...) const)
	{
		Slot<Arguments...> compareSlot(object, function);
		if (m_slot == compareSlot)
			m_slot.disconnect();
	}
	/**
	 * @brief Disconnect a function from the signal
	 * @param function The function which should disconnect.
	 */
	void disconnect(void (*function)(Arguments...))
	{
		Slot<Arguments...> compareSlot(function);
		if (m_slot == compareSlot)
			m_slot.disconnect();
	}
	/**
	 * @brief Call the connected methods or functions.
	 * @param arguments The arguments which should pass to the methods or functions.
	 */
	void emitSignal(Arguments... arguments) const
	{
		m_slot.emitSlot(std::forward<Arguments>(arguments)...);
	}
	/**
	 * @brief Call the connected methods or functions.
	 * @param arguments The arguments which should pass to the methods or functions.
	 */
	void operator() (Arguments... arguments) const
	{
		m_slot.emitSlot(std::forward<Arguments>(arguments)...);
	}
	/**
	 * @brief Checks if the \c Signal is connected to slot.
	 * @return \c true if no connection exists, \c false otherwise.
	 */
	bool empty() const
	{
		return !m_slot.connected();
	}
private:
	/**Slot for handling the callback invocation.*/
	Slot<Arguments...> m_slot;
};
}  // namespace semf
#endif  // CORE_SIGNALS_SIGNAL_H_
