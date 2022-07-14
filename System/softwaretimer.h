/**
 * @file softwaretimer.h
 *
 * @date 20.03.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SYSTEM_SOFTWARETIMER_H_
#define SYSTEM_SOFTWARETIMER_H_

#include <Interfaces/timer.h>
#include <Interfaces/tickreceiver.h>
#include <System/timebase.h>
#include <cstdint>

namespace semf
{
/**
 * @brief Software \c SoftwareTimer which can be added to a \c TimeBase or used singular.
 *
 * For using the object combined with a \c TimeBase, add the timer to the timebase,
 * for the singular usage case, call the count function e.g. in the microcontroller
 * hardware timer interrupt service routine.
 * Setting interval to zero will lead to the same behaviour as setting the interval to one.
 */
class SoftwareTimer : public Timer, public TickReceiver
{
public:
	/**
	 * @brief Constructor using no timebase.
	 * @param interval The timer interval.
	 * @param start \li true: SoftwareTimer start running. \li false: Time is not running.
	 */
	explicit SoftwareTimer(uint32_t interval = 0, bool start = false);

	/**
	 * @brief Constructor using no timebase.
	 * @param timeBase The timebase for the timer.
	 * @param interval The timer interval.
	 * @param start \li true: SoftwareTimer start running. \li false: Time is not running.
	 */
	explicit SoftwareTimer(TimeBase& timeBase, uint32_t interval = 0, bool start = false);

	/**
	 * Set the timer interval.
	 * @param interval The timer interval.
	 */
	void setInterval(uint32_t interval);

	/**
	 * Get the timer interval.
	 * @return The timer interval.
	 */
	uint32_t interval() const;

	/**
	 * Get the current counter value
	 * @return current counter value
	 */
	uint32_t counterValue() const;

	/**
	 * Restart counting.
	 */
	void reset();

	/**
	 * Counting the time.
	 * Must be called cyclically by a timer interrupt or similar.
	 */
	void tick();

	/**
	 * Start time counting.
	 * @remark Do not reset the time counter.
	 */
	void start();

	/**
	 * Stop time counting.
	 */
	void stop();

	/**
	 * Check if the timer is running
	 * @return \li true: timer is running.
	 * 		   \li false: timer is not running.
	 */
	bool isRunning() const;

private:
	bool m_running; /**< on true, timer is running*/
	uint32_t m_counter = 0; /**<for counting the time*/
	uint32_t m_interval; /**<for saving the timer interval.*/
};

} /* namespace semf */

#endif /* SYSTEM_SOFTWARETIMER_H_ */
