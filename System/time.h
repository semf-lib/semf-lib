/**
 * @file time.h
 *
 * @date 24.05.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SYSTEM_TIME_H_
#define SYSTEM_TIME_H_

#include <cstdint>
#include <cstddef>

namespace semf
{
/**
 * @brief The \c Time class provides clock time functions.
 *
 * A \c Time object contains a time expressed as the numbers of hours,
 * minutes, seconds, and milliseconds since midnight.
 * It provides functions for comparing two \c Time objects and for altering a \c Time object
 * by adding a number of milliseconds.
 *
 * \c Time uses the 24-hour clock format; it has no concept of AM/PM.
 * Unlike \c DateTime, \c Time knows nothing about time zones or daylight-saving
 * time (DST).
 *
 * A \c Time object is typically created either by giving the number of
 * hours, minutes, seconds, and milliseconds explicitly.
 *
 * The \c hours(), \c minutes(), seconds(), and milliseconds() functions provide
 * access to the number of hours, minutes, seconds, and milliseconds of the time.
 */
class Time
{
public:
	Time() = default;

	/**
	 * @brief Copy constructor.
	 * @param timeToSet Copy time information into \c this object.
	 * @note The \c Rtc interface will not be copied.
	 */
	Time(const Time& timeToSet);

	/**
	 * @brief Constructor for static time.
	 * @param millisecond Milliseconds for static time.
	 * @param second Seconds for static time.
	 * @param minute Minutes for static time.
	 * @param hour Hours for static time.
	 */
	Time(uint16_t millisecond, uint8_t second, uint8_t minute, uint8_t hour);

	/**
	 * @brief Sets date time information
	 *        and system clock if \c Rtc is registered.
	 * @param millisecond Milliseconds to set.
	 * @param second Seconds to set.
	 * @param minute Minutes to set.
	 * @param hour Hours to set.
	 * @return \c true for time is valid, \c false for not.
	 */
	bool setTime(uint16_t millisecond, uint8_t second, uint8_t minute, uint8_t hour);

	/**
	 * @brief Returns the milliseconds from system clock or static value.
	 * @return Millisecond setting.
	 */
	uint16_t millisecond() const;

	/**
	 * @brief Sets the milliseconds.
	 * @param millisecond Millisecond setting.
	 * @return \c true for time is valid, \c false for not.
	 */
	bool setMillisecond(uint16_t millisecond);

	/**
	 * @brief Returns the seconds from system clock or static value.
	 * @return Second setting.
	 */
	uint8_t second() const;

	/**
	 * @brief Sets the seconds.
	 * @param second Second setting.
	 * @return \c true for time is valid, \c false for not.
	 */
	bool setSecond(uint8_t second);

	/**
	 * @brief Returns the minutes.
	 * @return Minute setting.
	 */
	uint8_t minute() const;

	/**
	 * @brief Sets the minutes.
	 * @param minute Minutes setting.
	 * @return \c true for time is valid, \c false for not.
	 */
	bool setMinute(uint8_t minute);

	/**
	 * @brief Returns the hours.
	 * @return Hour setting.
	 */
	uint8_t hour() const;

	/**
	 * @brief Sets the hours.
	 * @param hour Hours setting.
	 * @return \c true for time is valid, \c false for not.
	 */
	bool setHour(uint8_t hour);

	/**
	 * @brief Adds milliseconds to \c this time.
	 * @param milliseconds Milliseconds to add.
	 */
	void addMilliseconds(int32_t milliseconds);

	/**
	 * @brief Adds seconds to \c this time.
	 * @param seconds Seconds to add.
	 */
	void addSeconds(int32_t seconds);

	/**
	 * @brief Adds minutes to \c this time.
	 * @param minutes Minutes to add.
	 */
	void addMinutes(int32_t minutes);

	/**
	 * @brief Adds hours to \c this time.
	 * @param hours Hours to add.
	 */
	void addHours(int32_t hours);

	/**
	 * @brief Returns the seconds since start of the day.
	 * @return Seconds since start of the day.
	 */
	uint32_t secondsSinceStartOfDay() const;

	/**
	 * @brief Returns the milliseconds since start of the day.
	 * @return Milliseconds since start of the day.
	 */
	uint32_t millisecondsSinceStartOfDay() const;

	/**
	 * @brief Returns a time difference between \c this time and \c subtract.
	 * @param subtract Time to be subtracted.
	 * @return time difference in milliseconds.
	 */
	int32_t timeSpan(const Time& subtract) const;

	/**
	 * @brief Copies the time.
	 * @param timeToSet Source to copy the time information from.
	 * @note The \c Rtc interface will not be copied.
	 * @return time object itself.
	 */
	Time& operator=(const Time& timeToSet);

	/**
	 * @brief Compares \c this time with \c other.
	 * @param timeToCompareWith time to compare.
	 * @return \c true if times are the same; \c false if dates are different.
	 */
	bool operator==(const Time& timeToCompareWith) const;

	/**
	 * @brief Compares \c this time with \c other.
	 * @param timeToCompareWith time to compare.
	 * @return \c true if times are different; \c false if dates are the same.
	 */
	bool operator!=(const Time& timeToCompareWith) const;

	/**
	 * @brief Compares \c this time with \c other.
	 * @param timeToCompareWith time to compare.
	 * @return \c true if \c this time is smaller; \c false if not.
	 */
	bool operator<(const Time& timeToCompareWith) const;

	/**
	 * @brief Compares \c this time with \c other.
	 * @param timeToCompareWith time to compare.
	 * @return \c true if \c this time is smaller or equal; \c false if not.
	 */
	bool operator<=(const Time& timeToCompareWith) const;

	/**
	 * @brief Compares \c this time with \c other.
	 * @param timeToCompareWith time to compare.
	 * @return \c true if \c this time is larger; \c false if not.
	 */
	bool operator>(const Time& timeToCompareWith) const;

	/**
	 * @brief Compares \c this time with \c other.
	 * @param timeToCompareWith time to compare.
	 * @return \c true if \c this time is larger or equal; \c false if not.
	 */
	bool operator>=(const Time& timeToCompareWith) const;

protected:
	/**
	 * @brief Returns if the given time is valid.
	 * @param millisecond Millisecond to check.
	 * @param second Second to check.
	 * @param minute Minute to check.
	 * @param hour Hour to check.
	 * @return \c true for time setting is valid, \c false for not.
	 */
	static bool isValid(uint16_t millisecond, uint8_t second, uint8_t minute, uint8_t hour);

private:
	/**Number of milliseconds*/
	uint16_t m_millisecond = 0;
	/**Number of seconds*/
	uint8_t m_second = 0;
	/**Number of minutes*/
	uint8_t m_minute = 0;
	/**Number of hours*/
	uint8_t m_hour = 0;
};

} /* namespace semf */

#endif /* SYSTEM_TIME_H_ */
