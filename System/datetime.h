/**
 * @file datetime.h
 *
 * @date 28.05.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SYSTEM_DATETIME_H_
#define SYSTEM_DATETIME_H_

#include <System/date.h>
#include <System/time.h>

namespace semf
{
/**
 * @brief The \c DateTime class provides date and time functionality as a point of time.
 * It combines features of the \c Date and \c Time classes (via inheritance).
 *
 * The date and time can be modified by
 * - \c setDateTime(),
 * - \c setDate() and
 * - \c setTime().
 *
 * \c DateTime provides a full set of operators to compare two \c DateTime objects where smaller means earlier,
 * and larger means later.
 */
class DateTime : public Date, public Time
{
public:
	DateTime() = default;

	/**
	 * @brief Copy Constructor.
	 * @param millisecond Milliseconds to set.
	 * @param second Seconds to set.
	 * @param minute Minutes to set.
	 * @param hour Hours to set.
	 * @param day Day to set.
	 * @param month Month to set.
	 * @param year Year to set.
	 */
	DateTime(uint16_t millisecond, uint8_t second, uint8_t minute,
			 uint8_t hour, uint8_t day, Month month, uint16_t year);

	/**
	 * @brief Sets date time information.
	 * @param unixTimeInSeconds Unix time in seconds.
	 */
	explicit DateTime(uint64_t unixTimeInSeconds);

	/**
	 * @brief Sets date time information
	 *        and system clock if \c Rtc is registered.
	 * @param millisecond Milliseconds to set.
	 * @param second Seconds to set.
	 * @param minute Minutes to set.
	 * @param hour Hours to set.
	 * @param day Day to set.
	 * @param month Month to set.
	 * @param year Year to set.
	 * @return \c true for DateTime is valid, \c false for not.
	 */
	bool setDateTime(uint16_t millisecond, uint8_t second, uint8_t minute,
			uint8_t hour, uint8_t day, Month month, uint16_t year);

	/**
	 * @brief Sets date time information.
	 * @param copyFromTime Time to set.
	 * @param copyFromDate Date to set.
	 * @return \c true for DateTime is valid, \c false for not.
	 */
	bool setDateTime(const Time& copyFromTime, const Date& copyFromDate);

	/**
	 * @brief Sets date time information.
	 * @param unixTimeInSeconds Unix time in seconds.
	 */
	void setDateTime(uint64_t unixTimeInSeconds);

	/**
	 * @brief Returns the date time in seconds since the 1 January 1970.
	 * @return unix time in seconds.
	 */
	uint64_t unixTimeInSeconds() const;

	/**
	 * @brief Adds hours to \c this datetime.
	 * @param hours Hours to add.
	 */
	void addHours(int32_t hours);

	/**
	 * @brief Compares \c this date time with \c other.
	 * @param other date time to compare.
	 * @return \c true if date times are the same; \c false if date times are different.
	 */
	bool operator==(const DateTime& other) const;

	/**
	 * @brief Compares \c this date time with \c other.
	 * @param other date time to compare.
	 * @return \c true if date times are different; \c false if date times are the same.
	 */
	bool operator!=(const DateTime& other) const;

	/**
	 * @brief Compares \c this date time with \c other.
	 * @param other date time to compare.
	 * @return \c true if \c this date time is smaller; \c false if not.
	 */
	bool operator<(const DateTime& other) const;

	/**
	 * @brief Compares \c this date time with \c other.
	 * @param other date time to compare.
	 * @return \c true if \c this date time is smaller or equal; \c false if not.
	 */
	bool operator<=(const DateTime& other) const;

	/**
	 * @brief Compares \c this date time with \c other.
	 * @param other date time to compare.
	 * @return \c true if \c this date time is larger; \c false if not.
	 */
	bool operator>(const DateTime& other) const;

	/**
	 * @brief Compares \c this date time with \c other.
	 * @param other date time to compare.
	 * @return \c true if \c this date time is larger or equal; \c false if not.
	 */
	bool operator>=(const DateTime& other) const;
};

} /* namespace semf */

#endif /* SYSTEM_DATETIME_H_ */
