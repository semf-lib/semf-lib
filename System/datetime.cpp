/**
 * @file datetime.cpp
 *
 * @date 28.05.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <System/datetime.h>
#include <Core/debug.h>

namespace semf
{
DateTime::DateTime(uint16_t millisecond, uint8_t second, uint8_t minute, uint8_t hour,
		uint8_t day, Month month, uint16_t year)
{
	setDateTime(millisecond, second, minute, hour, day, month, year);
}

DateTime::DateTime(uint64_t unixTimeInSeconds)
{
	setDateTime(unixTimeInSeconds);
}

bool DateTime::setDateTime(uint16_t millisecond, uint8_t second, uint8_t minute, uint8_t hour,
		uint8_t day, Month month, uint16_t year)
{
	if (Time::isValid(millisecond, second, minute, hour) && Date::isValid(day, month, year))
	{
		setDate(day, month, year);
		setTime(millisecond, second, minute, hour);
		return true;
	}
	else
	{
		SEMF_ERROR("invalid parameter in setDateTime: y %u; m %u; d %u; h %u; m %u; s %u; mi %u",
				year, static_cast<uint8_t>(month), day, hour, minute, second, millisecond);
		return false;
	}
}

bool DateTime::setDateTime(const Time& copyFromTime, const Date& copyFromDate)
{
	return setDateTime(copyFromTime.millisecond(), copyFromTime.second(), copyFromTime.minute(),
			copyFromTime.hour(), copyFromDate.day(), copyFromDate.month(), copyFromDate.year());
}

void DateTime::setDateTime(uint64_t unixTimeInSeconds)
{
	uint8_t second = 0, minute = 0, hour = 0, month = 1;
	uint16_t day = 365, year = 1970;

	// Second
	second = unixTimeInSeconds % 60;
	unixTimeInSeconds /= 60;
	// Minute
	minute = unixTimeInSeconds % 60;
	unixTimeInSeconds /= 60;
	// Hour
	hour = unixTimeInSeconds % 24;
	unixTimeInSeconds /= 24;
	// Year
	while (unixTimeInSeconds >= day)
	{
		unixTimeInSeconds -= day;
		day = isLeapYear(++year) ? 366 : 365;
	}
	// Month
	day = daysInMonth(static_cast<Month>(month), year);
	while (unixTimeInSeconds >= day)
	{
		unixTimeInSeconds -= day;
		day = daysInMonth(static_cast<Month>(++month), year);
	}
	// Day
	day = unixTimeInSeconds + 1;

	SEMF_INFO("setDateTime from unix-timestamp %u", unixTimeInSeconds);
	setDate(static_cast<uint8_t>(day), static_cast<Month>(month), year);
	setTime(0, second, minute, hour);
}

uint64_t DateTime::unixTimeInSeconds() const
{
	uint8_t sec = this->second();
	uint8_t min = this->minute();
	uint8_t hrs = this->hour();
	uint8_t day = this->day();
	uint8_t mon = this->month();
	uint16_t year = this->year();

	uint64_t ts = 0;
	uint8_t years = 0;
	uint8_t leapYears = 0;

	for (uint16_t i = 1970; i < year; i++)
	{
		if (isLeapYear(i))
		{
			leapYears++;
		}
		else
		{
			years++;
		}
	}
	ts += ( (years*365) + (leapYears*366) ) * static_cast<uint64_t>(86400);

	//  Add up the seconds from all prev days this year, up until today.
	for (uint8_t i = static_cast<uint8_t>(January); i < mon; i++)
	{
		//  days from previous months this year
		ts += daysInMonth(static_cast<Month>(i), year) * 86400;
	}
	ts += (day-1) * 86400;  // days from this month

	//  Calculate seconds elapsed just today.
	ts += hrs * 3600;
	ts += min * 60;
	ts += sec;

	SEMF_INFO("unixTimeInSeconds %u out of y %u; m %u; d %u; h %u; m %m; s %u",
			ts, year, mon, day, hrs, min, sec);
	return ts;
}

void DateTime::addHours(int32_t hours)
{
	int32_t adddays = 0;

	if (hours >= 0)
	{
		adddays = hours / 24;
		hours -= adddays * 24;
		adddays += (hour() + hours) / 24;
		setHour((hour() + hours) % 24);
	}
	else
	{
		if ((static_cast<int32_t>(hour()) + hours) >= 0)
		{
			setHour((static_cast<int32_t>(hour()) + hours) % 24);
		}
		else
		{
			adddays = ((hours + hour()) / 24) - 1;
			setHour(24 + ((static_cast<int32_t>(hour()) + hours) % 24));
		}
	}
	SEMF_INFO("addHours to %u", hour());
	addDays(adddays);
}

bool DateTime::operator==(const DateTime& other) const
{
	if (Date::operator==(static_cast<Date>(other)) &&
			Time::operator==(static_cast<Time>(other)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DateTime::operator!=(const DateTime& other) const
{
	return !(*this == other);
}

bool DateTime::operator<(const DateTime& other) const
{
	if (Date::operator<(static_cast<Date>(other)) ||
			(Date::operator==(static_cast<Date>(other)) &&
					Time::operator<(static_cast<Time>(other))))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DateTime::operator<=(const DateTime& other) const
{
	return (*this < other || *this == other);
}

bool DateTime::operator>(const DateTime& other) const
{
	return !(*this <= other);
}

bool DateTime::operator>=(const DateTime& other) const
{
	return !(*this < other);
}

} /* namespace semf */
