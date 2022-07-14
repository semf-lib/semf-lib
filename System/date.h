/**
 * @file date.h
 *
 * @date 24.05.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SYSTEM_DATE_H_
#define SYSTEM_DATE_H_

#include <cstddef>
#include <cstdint>

namespace semf
{
/**
 * @brief The \c Date class provides date functions.
 *
 * A \c Date object represents a particular date, i.e. a calendar date with year, month, and day numbers.
 *
 * A \c Date object is typically created by giving the year, month, and day numbers explicitly.
 * An explicit date can also be set using \c setDate().
 * The \c year(), \c month(), and \c day() functions provide access to the year, month, and day numbers.
 *
 * \c Date provides a full set of operators to compare two Date objects where smaller means earlier,
 * and larger means later.
 *
 * \note Note that \c Date interprets year numbers less than 100 literally, i.e. as years 1 through 99,
 * without adding any offset.
 */
class Date
{
public:
	/**
	 * @brief Enumeration of months.
	 *
	 * Starting with January is 1, December is 12
	 */
	enum Month
	{
		January = 1,
		February,
		March,
		April,
		May,
		June,
		July,
		August,
		September,
		October,
		November,
		December
	};

	/**
	 * @brief Enumeration of days in a week.
	 *
	 * Starting with Sunday is 0, Saturday is 6
	 */
	enum Weekday
	{
		Sunday = 0,
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday
	};

	Date() = default;

	/**
	 * @brief Constructor.
	 * @param day Day to set.
	 * @param month Month to set.
	 * @param year Year to set.
	 */
	Date(uint8_t day, Month month, uint16_t year);

	/**
	 * @brief Sets date information.
	 * @param day Day to set.
	 * @param month Month to set.
	 * @param year Year to set.
	 * @return \c true for date setting is valid, \c false for not.
	 */
	bool setDate(uint8_t day, Month month, uint16_t year);

	/**
	 * @brief Returns the day.
	 * @return Day setting.
	 */
	uint8_t day() const;

	/**
	 * @brief Sets a day.
	 * @param day Day setting.
	 * @return \c true for day setting is valid, \c false for not.
	 */
	bool setDay(uint8_t day);

	/**
	 * @brief Returns the month.
	 * @return Month setting.
	 */
	Month month() const;

	/**
	 * @brief Sets a month.
	 * @param month Month setting.
	 * @return \c true for month setting is valid, \c false for not.
	 */
	bool setMonth(Month month);

	/**
	 * @brief Returns the year.
	 * @return Year setting.
	 */
	uint16_t year() const;

	/**
	 * @brief Sets a year.
	 * @param year Year setting.
	 * @return \c true for year setting is valid, \c false for not.
	 */
	bool setYear(uint16_t year);

	/**
	 * @brief Adds days to \c this date.
	 * @param days Days to add.
	 */
	void addDays(int32_t days);

	/**
	 * @brief Returns the day in the week of the actual \c Date.
	 * @return Day in week.
	 * @see \c Weekday
	 */
	Weekday weekday() const;

	/**
	 * @brief Adds months to \c this date.
	 * @param months Months to add.
	 */
	void addMonths(int32_t months);

	/**
	 * @brief Adds years to \c this date.
	 * @param years Years to add.
	 */
	void addYears(int32_t years);

	/**
	 * @brief Compares \c this date with \c other.
	 * @param dateToCompareWith date to compare.
	 * @return \c true if dates are the same; \c false if dates are different.
	 */
	bool operator==(const Date& dateToCompareWith) const;

	/**
	 * @brief Compares \c this date with \c other.
	 * @param dateToCompareWith date to compare.
	 * @return \c true if dates are different; \c false if dates are the same.
	 */
	bool operator!=(const Date& dateToCompareWith) const;

	/**
	 * @brief Compares \c this date with \c other.
	 * @param dateToCompareWith date to compare.
	 * @return \c true if \c this date is smaller; \c false if not.
	 */
	bool operator<(const Date& dateToCompareWith) const;

	/**
	 * @brief Compares \c this date with \c other.
	 * @param dateToCompareWith date to compare.
	 * @return \c true if \c this date is smaller or equal; \c false if not.
	 */
	bool operator<=(const Date& dateToCompareWith) const;

	/**
	 * @brief Compares \c this date with \c other.
	 * @param dateToCompareWith date to compare.
	 * @return \c true if \c this date is larger; \c false if not.
	 */
	bool operator>(const Date& dateToCompareWith) const;

	/**
	 * @brief Compares \c this date with \c other.
	 * @param dateToCompareWith date to compare.
	 * @return \c true if \c this date is larger or equal; \c false if not.
	 */
	bool operator>=(const Date& dateToCompareWith) const;

protected:
	/**
	 * @brief Returns the number of days in a specific month and year.
	 *
	 * Returns "0" for invalid month and year setting.
	 *
	 * @param month Month to get the number of days from.
	 * @param year Year in which the month is in.
	 * @return Number of days.
	 */
	static uint8_t daysInMonth(Month month, uint16_t year);

	/**
	 * @brief Returns if a specific year is a leap year or not.
	 * @param year Year to check leap lear.
	 * @return \c true for is a leap year, \c false for is no leap year.
	 */
	static bool isLeapYear(uint16_t year);

	/**
	 * @brief Returns if the given date is valid.
	 * @param day Day to check.
	 * @param month Month to check.
	 * @param year Year to check.
	 * @return \c true for date setting is valid, \c false for not.
	 */
	static bool isValid(uint8_t day, Month month, uint16_t year);

private:
	/**Number of days*/
	uint8_t m_day = 1;
	/**Number of month*/
	Month m_month = Month::January;
	/**Number of year*/
	uint16_t m_year = 0;
};

} /* namespace semf */

#endif /* SYSTEM_DATE_H_ */
