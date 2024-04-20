/**
 * @file timebase.h
 * @date 26.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_SYSTEM_TIMEBASE_H_
#define SEMF_SYSTEM_TIMEBASE_H_

#include <semf/app/system/timebase.h>
#include <semf/app/system/timer.h>
#include <semf/utils/core/lists/linkedlist.h>
#include <semf/utils/core/signals/slot.h>

namespace semf
{
/**
 * @copydoc app::TimeBase
 */
class TimeBase : public app::TimeBase
{
public:
	/**
	 * @brief Constructor.
	 * @param timer Timer interface (e.g. hardware timer).
	 * @param enable Enables the \c TimeBase.
	 */
	explicit TimeBase(app::Timer& timer, bool enable = false);
	explicit TimeBase(const TimeBase& other) = delete;
	//! @cond Doxygen_Suppress
	virtual ~TimeBase() = default;
	//! @endcond

	/**
	 * @brief Adds e.g. a timer to this \c TimeBase.
	 * @param tickReceiver e.g. /c SoftwareTimer or /c DigitalInPolling to add.
	 */
	void add(TickReceiver& tickReceiver) override;
	/**
	 * @brief Removes e.g. a timer from this \c TimeBase.
	 * @param tickReceiver e.g. /c SoftwareTimer or /c DigitalInPolling to remove.
	 */
	void remove(TickReceiver& tickReceiver) override;
	/**
	 * @brief Enables the \c TimeBase.
	 * @param enable \c true enables the \c TimeBase.
	 * @note Only an enabled timebase's timers counts and elapses.
	 */
	void enable(bool enable = true) override;
	/**
	 * @brief Disables the \c TimeBase.
	 * @param disable \c true disables the \c TimeBase.
	 * @note Only an enabled timebase's timers counts and elapses.
	 */
	void disable(bool disable = true) override;
	/**
	 * @brief Returns if the \c TimeBase is enabled.
	 *
	 * Only a enabled timebase's timers count and elapse.
	 * @return \c true for \c TimeBase enabled, otherwise \c false.
	 */
	bool isEnabled() const override;

private:
	/**
	 * @brief Counting the time of all timers registered in this \c TimeBase.
	 * @attention Must be called cyclically by a timer interrupt or similar.
	 */
	void count();

	/**On true, time-base is enabled.*/
	bool m_enabled;
	/**Hardware timer.*/
	app::Timer* m_timer;
	/**Slot for count function.*/
	SEMF_SLOT(m_countSlot, TimeBase, *this, count);
	/**List of tick receivers.*/
	LinkedList<TickReceiver> m_tickReceiverList;
};
} /* namespace semf */
#endif /* SEMF_SYSTEM_TIMEBASE_H_ */
