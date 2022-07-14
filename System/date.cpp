/**
 * @file date.cpp
 *
 * @date 24.05.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <System/date.h>
#include <Core/debug.h>

namespace semf
{
Date::Date(uint8_t day, Month month, uint16_t year)
{
	setDate(day, month, year);
}

bool Date::setDate(uint8_t day, Month month, uint16_t year)
{
	bool isvalid = isValid(day, month, year);
	if (isvalid)
	{
		m_day = day;
		m_month = month;
		m_year = year;
		SEMF_INFO("setDate to: y %u; m %u; d %u", year, static_cast<uint8_t>(month), day);
	}
	else
	{
		SEMF_ERROR("invalid parameter in setDate: y %u; m %u; d %u",
				year, static_cast<uint8_t>(month), day);
	}
	return isvalid;
}

uint8_t Date::day() const
{
	return m_day;
}

bool Date::setDay(uint8_t day)
{
	bool isvalid = isValid(day, month(), year());
	if (isvalid)
	{
		m_day = day;
		SEMF_INFO("setDay to %u", day);
	}
	else
	{
		SEMF_ERROR("invalid parameter in setDay: y %u; m %u; d %u",
				year(), static_cast<uint8_t>(month()), day);
	}
	return isvalid;
}

Date::Month Date::month() const
{
	return m_month;
}

bool Date::setMonth(Month month)
{
	bool isvalid = isValid(day(), month, year());
	if (isvalid)
	{
		m_month = month;
		SEMF_INFO("setMonth to %u", month);
	}
	else
	{
		SEMF_ERROR("invalid parameter in setDay: y %u; m %u; d %u",
				year(), static_cast<uint8_t>(month), day());
	}
	return isvalid;
}

uint16_t Date::year() const
{
	return m_year;
}

bool Date::setYear(uint16_t year)
{
	bool isvalid = isValid(day(), month(), year);
	if (isvalid)
	{
		m_year = year;
		SEMF_INFO("setYear to %u", year);
	}
	else
	{
		SEMF_ERROR("invalid parameter in setDay: y %u; m %u; d %u",
				year, static_cast<uint8_t>(month()), day());
	}
	return isvalid;
}

void Date::addDays(int32_t days)
{
	int32_t addmonths = 0;
	uint8_t m = month();
	uint16_t y = year();
	uint8_t daysinmonth = daysInMonth(static_cast<Month>(m), y);

	if (days >= 0)
	{
		while (days + day() > daysinmonth)
		{
			days -= daysinmonth;
			if (++m > 12)
			{
				m = 1;
				y++;
			}
			addmonths++;
			daysinmonth = daysInMonth(static_cast<Month>(m), y);
		}
	}
	else
	{
		while (days + day() < 1)
		{
			if (--m == 0)
			{
				m = 12;
				if (y > 0)
					y--;
			}
			daysinmonth = daysInMonth(static_cast<Month>(m), y);
			days += daysinmonth;
			addmonths--;
		}
	}
	m_day += days;
	SEMF_INFO("addDays to %u", m_day);
	addMonths(addmonths);
}

void Date::addMonths(int32_t months)
{
	int32_t addyears = 0;

	if (months >= 0)
	{
		addyears = months / 12;
		months -= addyears * 12;
		addyears += (month() + months - 1) / 12;
		m_month = static_cast<Month>(((month() + months - 1) % 12) + 1);
	}
	else
	{
		if (months + static_cast<uint8_t>(month()) < 1)
		{
			addyears = ((months + static_cast<uint8_t>(month()) - 1) / 12) - 1;
			months -= addyears * 12;
		}
		m_month = static_cast<Month>(months + static_cast<uint8_t>(month()));
		SEMF_INFO("addMonths to %u", m_month);
	}
	addYears(addyears);
}

void Date::addYears(int32_t years)
{
	if (static_cast<int32_t>(year()) + years >= 0)
	{
		m_year += years;
	}
	else
	{
		m_year = 0;
	}
	SEMF_INFO("addYears to %u", m_year);
}

Date::Weekday Date::weekday() const
{
    // URL https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week#Sakamoto's_methods

    static const uint8_t t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    uint32_t y = m_year;
    y -= static_cast<uint8_t>(m_month) < 3;
    return static_cast<Weekday>((y + y/4 - y/100 + y/400 + t[static_cast<uint8_t>(m_month)-1] + m_day) % 7);
}

bool Date::operator==(const Date& dateToCompareWith) const
{
	if (year() == dateToCompareWith.year() &&
			month() == dateToCompareWith.month() &&
			day() == dateToCompareWith.day())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Date::operator!=(const Date& dateToCompareWith) const
{
	return !(*this == dateToCompareWith);
}

bool Date::operator<(const Date& dateToCompareWith) const
{
	if (year() < dateToCompareWith.year() ||
			(year() == dateToCompareWith.year() && month() < dateToCompareWith.month()) ||
			(year() == dateToCompareWith.year() && month() == dateToCompareWith.month() &&
					day() < dateToCompareWith.day()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Date::operator<=(const Date& dateToCompareWith) const
{
	return (*this < dateToCompareWith || *this == dateToCompareWith);
}

bool Date::operator>(const Date& dateToCompareWith) const
{
	return !(*this <= dateToCompareWith);
}

bool Date::operator>=(const Date& dateToCompareWith) const
{
	return !(*this < dateToCompareWith);
}

uint8_t Date::daysInMonth(Month month, uint16_t year)
{
	// Jan., Mar., Mai., Jul., Aug., Okt., Dec.
	if (month == January || month == March || month == May || month == July ||
			month == August || month == October || month == December)
	{
		return 31;
	}
	// Apr., Jun., Sep., Nov.
	else if (month == April || month == June || month == September || month == November)
	{
		return 30;
	}
	// Feb.
	else if (month == February)
	{
		if (isLeapYear(year))
		{
			return 29;
		}
		else
		{
			return 28;
		}
	}
	else
	{
		return 0;
	}
}

bool Date::isLeapYear(uint16_t year)
{
	return (((year)%4 == 0) && ((year)%100 != 0)) || ((year)%400 == 0);
}

bool Date::isValid(uint8_t day, Month month, uint16_t year)
{
	uint8_t daysinmonth = daysInMonth(month, year);
	return (daysinmonth > 0 && day != 0 && day <= daysinmonth) ? true : false;
}

} /* namespace semf */
