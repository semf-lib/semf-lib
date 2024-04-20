/**
 * @file slot.h
 *
 * @date 11.05.2023
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory
 */

#ifndef SEMF_UTILS_CORE_SIGNALS_SLOT_H_
#define SEMF_UTILS_CORE_SIGNALS_SLOT_H_

#include <semf/utils/core/signals/slotbase.h>
#include <utility>

/*!
	@brief Creates a binding to a given member function.
	@param function Member function to bind to.
*/
#define SEMF_SLOT_FUNC(function) [](auto& obj, auto&&... args) { obj.function(std::forward<decltype(args)>(args)...); }
/*!
	@brief Creates a semf slot.
	@param name	Name of the created slot variable.
	@param className Name of the class.
	@param object Object of the class.
	@param function Member function to bind to.
	@param ... Additional parameter types.
*/
#define SEMF_SLOT(name, className, object, function, ...) semf::Slot<className, ##__VA_ARGS__> name = {object, SEMF_SLOT_FUNC(function)}

namespace semf
{
/**
 * @brief Slot for lightweight signal/slot implementation.
 * This ConstSlot is for connecting a signal to a member function.
 * When the signal is emitted, the given member function is invoked.
 * @param T Type of the class object
 * @param Arguments The parameter types for pass parameter over the signal to a method.
 */
template <typename T, typename... Arguments>
class Slot : public SlotBase<Arguments...>
{
public:
	/** typedef for the function pointer.*/
	typedef void (*Function)(T&, Arguments&&...);
	/**
	 * @brief Constructor
	 * @param object The class object.
	 * @param function The method which should called.
	 */
	Slot(T& object, Function function)
	: m_object(&object),
	  m_function(function)
	{
	}
	virtual ~Slot() = default;
	/**
	 * @brief Set the reference to the context.
	 * @param object The class object.
	 */
	void setObject(T& object);
	/**
	 * @brief Get the reference to the context
	 * @return Reference to the class object
	 */
	T& object() const;
	/**
	 * @brief setFunction Set the method which should called.
	 * @param function Pointer to the callback method
	 */
	void setFunction(Function function);
	/**
	 * @brief Get the method which should called.
	 * @return Pointer to the callback method.
	 */
	Function function() const;

protected:
	void call(Arguments&&... arguments);

private:
	/** callback context.*/
	T* m_object;
	/** callback function.*/
	Function m_function;
};

template <typename T, typename... Arguments>
void Slot<T, Arguments...>::setObject(T& object)
{
	m_object = &object;
}

template <typename T, typename... Arguments>
T& Slot<T, Arguments...>::object() const
{
	return *m_object;
}

template <typename T, typename... Arguments>
void Slot<T, Arguments...>::setFunction(Function function)
{
	m_function = function;
}

template <typename T, typename... Arguments>
typename Slot<T, Arguments...>::Function Slot<T, Arguments...>::function() const
{
	return m_function;
}

template <typename T, typename... Arguments>
void Slot<T, Arguments...>::call(Arguments&&... arguments)
{
	(m_function)(*m_object, std::forward<Arguments>(arguments)...);
}

}  // namespace semf
#endif  // SEMF_UTILS_CORE_SIGNALS_SLOT_H_
