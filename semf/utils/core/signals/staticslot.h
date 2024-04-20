/**
 * @file staticslot.h
 *
 * @date 11.05.2023
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory
 */

#ifndef SEMF_UTILS_CORE_SIGNALS_STATICSLOT_H_
#define SEMF_UTILS_CORE_SIGNALS_STATICSLOT_H_

#include <semf/utils/core/signals/slotbase.h>
#include <utility>

namespace semf
{
/**
 * @brief StaticSlot for lightweight signal/slot implementation.
 * This StaticSlot ist for connecting a signal to a static function which have a
 * return value from type void.
 * @param Arguments The parameter types for pass parameter over the signal to a function.
 */
template <typename... Arguments>
class StaticSlot : public SlotBase<Arguments...>
{
public:
	/** typedef for the function pointer.*/
	typedef void (*Function)(Arguments...);

	/**
	 * @brief Constructor
	 * @param function pointer to the callback function
	 */
	explicit StaticSlot(Function function);
	virtual ~StaticSlot() = default;
	/**
	 * @brief Set the callback function
	 * @param function The callback function
	 */
	void setFunction(Function function);
	/**
	 * @brief function Get the pointer to the callback function.
	 * @return The pointer to the callback function.
	 */
	Function function() const;

protected:
	void call(Arguments&&... arguments);

private:
	/** callback function.*/
	Function m_function;
};

template <typename... Arguments>
StaticSlot<Arguments...>::StaticSlot(Function function)
: m_function(function)
{
}

template <typename... Arguments>
void StaticSlot<Arguments...>::setFunction(Function function)
{
	m_function = function;
}

template <typename... Arguments>
typename StaticSlot<Arguments...>::Function StaticSlot<Arguments...>::function() const
{
	return m_function;
}

template <typename... Arguments>
void StaticSlot<Arguments...>::call(Arguments&&... arguments)
{
	(*m_function)(std::forward<Arguments>(arguments)...);
}

}  // namespace semf
#endif  // SEMF_UTILS_CORE_SIGNALS_STATICSLOT_H_
