/**
 * @file rtc.h
 *
 * @date 24.05.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef INTERFACES_RTC_H_
#define INTERFACES_RTC_H_

#include <Core/Signals/signal.h>
#include <Core/error.h>
#include <cstdint>

namespace semf
{
/**
 * @brief Interface for the real time clock
 */
class Rtc
{
public:
	virtual ~Rtc() = default;

	/**
	 * @brief Get the milliseconds from the current time.
	 * @return The milliseconds from the current time.
	 */
	virtual uint16_t millisecond() = 0;
	/**
	 * @brief For set a new time.
	 * @param millisecond The milliseconds for the new time.
	 */
	virtual void setMillisecond(uint16_t millisecond) = 0;
	/**
	 * @brief Get the seconds from the current time.
	 * @return The seconds from the current time.
	 */
	virtual uint8_t second() const = 0;
	/**
	 * @brief For set a new time.
	 * @param second The second for the new time.
	 */
	virtual void setSecond(uint8_t second) = 0;
	/**
	 * @brief Get the minutes from the current time.
	 * @return The minutes from the current time.
	 */
	virtual uint8_t minute() const = 0;
	/**
	 * @brief For set a new time.
	 * @param minute The minutes for the new time.
	 */
	virtual void setMinute(uint8_t minute) = 0;
	/**
	 * @brief Get the hours from the current time.
	 * @return The hours from the current time.
	 */
	virtual uint8_t hour() const = 0;
	/**
	 * @brief For set a new time.
	 * @param hour The hours for the new time.
	 */
	virtual void setHour(uint8_t hour) = 0;
	/**
	 * Get the current day.
	 * @return The current day.
	 */
	virtual uint8_t day() const = 0;
	/**
	 * @brief For set a new date.
	 * @param day The new day.
	 */
	virtual void setDay(uint8_t day) = 0;
	/**
	 * @brief Get the current month.
	 * @return The current month.
	 */
	virtual uint8_t month() const = 0;
	/**
	 * @brief For set a new date.
	 * @param month The new month.
	 */
	virtual void setMonth(uint8_t month) = 0;
	/**
	 * @brief Get the current year.
	 * @return The current year.
	 */
	virtual uint16_t year() const = 0;
	/**
	 * @brief For set a new date.
	 * @param year The new year.
	 */
	virtual void setYear(uint16_t year) = 0;

	/**Signal is emitted after an error in the hardware occurred.*/
	Signal<1, Error> error;
};

}  /* namespace semf */

#endif  /* INTERFACES_RTC_H_ */
