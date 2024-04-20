/**
 * @file slotbase.h
 *
 * @date 11.05.2023
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory
 */

#ifndef SEMF_UTILS_CORE_SIGNALS_SLOTBASE_H_
#define SEMF_UTILS_CORE_SIGNALS_SLOTBASE_H_

#include <utility>
namespace semf
{
template <typename... Arguments>
class Signal;
/**
 * @brief Base Class for all Slot implementations.
 * @tparam Arguments Parameter types of the connected callback.
 */
template <typename... Arguments>
class SlotBase
{
public:
	virtual ~SlotBase() = default;

protected:
	/**
	 * @brief The invocation of the callback.
	 * @param arguments Arguments for the callback.
	 */
	virtual void call(Arguments&&... arguments) = 0;

private:
	/**Pointer to the next slot.*/
	SlotBase<Arguments...>* m_nextSlot = nullptr;
	/*!
	 *  @brief Signal is friend.
	 *  @tparam Arguments Parameter types of the connected callback.
	 */
	friend class Signal<Arguments...>;
};
}  // namespace semf
#endif  // SEMF_UTILS_CORE_SIGNALS_SLOTBASE_H_
