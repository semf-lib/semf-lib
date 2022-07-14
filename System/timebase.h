/**
 * @file timebase.h
 *
 * @date 26.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SYSTEM_TIMEBASE_H_
#define SYSTEM_TIMEBASE_H_

#include <Core/Lists/linkedlist.h>
#include <Interfaces/timer.h>

namespace semf
{
class TickReceiver;
/**
 * @brief A \c TimeBase is the bridge between e.g. a hardware timer (interrupt service routine)
 * and \c TickReceiver objects like /c SoftwareTimer. Multiple timers can be added to one \c TimeBase
 * object, but one
 * \c TickReceiver object can only be added to a single \c TimeBase. It's also not possible to add
 * a \c TickReceiver object twice to a \c TimeBase.
 *
 * When e.g. the hardware timer interrupt elapses, the count function of every previously added
 * \c TickReceiver will be called in the order, the \c SoftwareTimer objects were added.
 *
 * A \c TimeBase can easily be \c enabled or \c disabled. A disabled \c TimeBase does not call
 * the timers count functions until it's enabled again.
 *
 * @attention: The \c count function must be called cyclically e.g. by a the interrupt service
 * routine of a microcontroller.
 */
class TimeBase
{
public:
	/**
	 * @brief Constructor.
	 * @param timer Timer interface (e.g. hardware timer).
	 * @param enable Enables the timebase.
	 */
	explicit TimeBase(Timer& timer, bool enable = false);

	/**
	 * @brief Connects the \c TimeBase to the \c Timer interface.
	 * @param timer Timer interface (e.g. hardware timer).
	 */
	void setTimer(Timer& timer);

	/**
	 * @brief Adds e.g. a timer to this timebase.
	 * @param tickReceiver e.g. /c SoftwareTimer or /c DigitalInPolling to add.
	 */
	void add(TickReceiver& tickReceiver);

	/**
	 * @brief Removes e.g. a timer from this timebase.
	 * @param tickReceiver e.g. /c SoftwareTimer or /c DigitalInPolling to remove.
	 */
	void remove(TickReceiver& tickReceiver);

	/**
	 * @brief Enables the \c TimeBase.
	 * @param enable \c true enables the \c TimeBase.
	 * @note Only an enabled timebase's timers counts and elapses.
	 */
	void setEnabled(bool enable = true);

	/**
	 * @brief Disables the \c TimeBase.
	 * @param disable \c true disables the \c TimeBase.
	 * @note Only an enabled timebase's timers counts and elapses.
	 */
	void setDisabled(bool disable = true);

	/**
	 * @brief Returns if the \c TimeBase is enabled.
	 *
	 * Only a enabled timebase's timers count and elapse.
	 * @return \c true for \c TimeBase enabled, otherwise \c false.
	 */
	bool isEnabled() const;

private:
	/**
	 * @brief Counting the time of all timers registered in this timebase.
	 * @attention Must be called cyclically by a timer interrupt or similar.
	 */
	void count();

	bool m_enabled; /**< on true, timebase is enabled*/
	LinkedList<TickReceiver> m_tickReceiverList; /**< List of tick receivers*/
};

} /* namespace semf */

#endif /* SYSTEM_TIMEBASE_H_ */
