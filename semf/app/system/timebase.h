/**
 * @file timebase.h
 * @date 07.06.2023
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_APP_SYSTEM_TIMEBASE_H_
#define SEMF_APP_SYSTEM_TIMEBASE_H_

#include <semf/system/tickreceiver.h>

namespace semf
{
namespace app
{
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
	TimeBase() = default;
	explicit TimeBase(const TimeBase& other) = delete;
	virtual ~TimeBase() = default;

	/**
	 * @brief Adds e.g. a timer to this timebase.
	 * @param tickReceiver e.g. /c SoftwareTimer or /c DigitalInPolling to add.
	 */
	virtual void add(TickReceiver& tickReceiver) = 0;
	/**
	 * @brief Removes e.g. a timer from this timebase.
	 * @param tickReceiver e.g. /c SoftwareTimer or /c DigitalInPolling to remove.
	 */
	virtual void remove(TickReceiver& tickReceiver) = 0;
	/**
	 * @brief Enables the \c TimeBase.
	 * @param enable \c true enables the \c TimeBase.
	 * @note Only an enabled timebase's timers counts and elapses.
	 */
	virtual void enable(bool enable = true) = 0;
	/**
	 * @brief Disables the \c TimeBase.
	 * @param disable \c true disables the \c TimeBase.
	 * @note Only an enabled timebase's timers counts and elapses.
	 */
	virtual void disable(bool disable = true) = 0;
	/**
	 * @brief Returns if the \c TimeBase is enabled.
	 *
	 * Only a enabled timebase's timers count and elapse.
	 * @return \c true for \c TimeBase enabled, otherwise \c false.
	 */
	virtual bool isEnabled() const = 0;
};
} /* namespace app */
} /* namespace semf */
#endif /* SEMF_APP_SYSTEM_TIMEBASE_H_ */
