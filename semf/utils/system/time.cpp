/**
 * @file time.cpp
 * @date 24.05.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/utils/core/debug.h>
#include <semf/utils/system/time.h>

namespace semf
{
Time::Time(const Time& timeToSet)
{
	setTime(timeToSet.millisecond(), timeToSet.second(), timeToSet.minute(), timeToSet.hour());
}

Time::Time(uint16_t millisecond, uint8_t second, uint8_t minute, uint8_t hour)
{
	setTime(millisecond, second, minute, hour);
}

bool Time::setTime(uint16_t millisecond, uint8_t second, uint8_t minute, uint8_t hour)
{
	if (isValid(millisecond, second, minute, hour))
	{
		m_millisecond = millisecond;
		m_second = second;
		m_minute = minute;
		m_hour = hour;
		SEMF_INFO("setTime to: h %u; m %u; s %u; mi %u", hour, minute, second, millisecond);
		return true;
	}
	else
	{
		SEMF_ERROR("invalid parameter in setTime: h %u; m %u; s %u; mi %u", hour, minute, second, millisecond);
		return false;
	}
}

uint16_t Time::millisecond() const
{
	return m_millisecond;
}

bool Time::setMillisecond(uint16_t millisecond)
{
	if (millisecond < 1000)
	{
		m_millisecond = millisecond;
		SEMF_INFO("setMillisecond to %u", millisecond);
		return true;
	}
	else
	{
		SEMF_ERROR("invalid parameter in setMillisecond: %u", millisecond);
		return false;
	}
}

uint8_t Time::second() const
{
	return m_second;
}

bool Time::setSecond(uint8_t second)
{
	if (second < 60)
	{
		m_second = second;
		SEMF_INFO("setSecond to %u", second);
		return true;
	}
	else
	{
		SEMF_ERROR("invalid parameter in setSecond: %u", second);
		return false;
	}
}

uint8_t Time::minute() const
{
	return m_minute;
}

bool Time::setMinute(uint8_t minute)
{
	if (minute < 60)
	{
		m_minute = minute;
		SEMF_INFO("setMinute to %u", minute);
		return true;
	}
	else
	{
		SEMF_ERROR("invalid parameter in setMinute: %u", minute);
		return false;
	}
}

uint8_t Time::hour() const
{
	return m_hour;
}

bool Time::setHour(uint8_t hour)
{
	if (hour < 24)
	{
		m_hour = hour;
		SEMF_INFO("setHour to %u", hour);
		return true;
	}
	else
	{
		SEMF_ERROR("invalid parameter in setHour: %u", hour);
		return false;
	}
}

void Time::addMilliseconds(int32_t milliseconds)
{
	int32_t addsec = 0;

	if (milliseconds >= 0)
	{
		addsec = milliseconds / 1000;
		milliseconds -= addsec * 1000;
		addsec += (millisecond() + milliseconds) / 1000;
		m_millisecond = static_cast<uint16_t>((m_millisecond + milliseconds) % 1000);
	}
	else
	{
		if ((static_cast<int32_t>(millisecond()) + milliseconds) >= 0)
		{
			m_millisecond = static_cast<uint16_t>((milliseconds + m_millisecond) % 1000);
		}
		else
		{
			addsec = ((milliseconds + millisecond()) / 1000) - 1;
			m_millisecond = static_cast<uint16_t>((milliseconds + m_millisecond + 1000) % 1000);
		}
	}
	SEMF_INFO("addMilliseconds to %u", m_millisecond);
	addSeconds(addsec);
}

void Time::addSeconds(int32_t seconds)
{
	int32_t addmin = 0;

	if (seconds >= 0)
	{
		addmin = seconds / 60;
		seconds -= addmin * 60;
		addmin += (second() + seconds) / 60;
		m_second = static_cast<uint8_t>((m_second + seconds) % 60);
	}
	else
	{
		if ((static_cast<int32_t>(second()) + seconds) >= 0)
		{
			m_second = static_cast<uint8_t>((seconds + m_second) % 60);
		}
		else
		{
			addmin = ((seconds + second()) / 60) - 1;
			m_second = static_cast<uint8_t>(((seconds + m_second) % 60) + 60);
		}
	}
	SEMF_INFO("addSeconds to %u", m_second);
	addMinutes(addmin);
}

void Time::addMinutes(int32_t minutes)
{
	int32_t addhours = 0;

	if (minutes >= 0)
	{
		addhours = minutes / 60;
		minutes -= addhours * 60;
		addhours += (minute() + minutes) / 60;
		m_minute = static_cast<uint8_t>((m_minute + minutes) % 60);
	}
	else
	{
		if ((static_cast<int32_t>(minute()) + minutes) >= 0)
		{
			m_minute = static_cast<uint8_t>((minutes + m_minute) % 60);
		}
		else
		{
			addhours = ((minutes + minute()) / 60) - 1;
			m_minute = static_cast<uint8_t>((minutes + m_minute % 60) + 60);
		}
	}
	SEMF_INFO("addMinutes to %u", m_minute);
	addHours(addhours);
}

void Time::addHours(int32_t hours)
{
	if (hours >= 0)
	{
		m_hour = static_cast<uint8_t>((hours + m_hour) % 24);
	}
	else
	{
		if ((static_cast<int32_t>(hour()) + hours) >= 0)
		{
			m_hour = static_cast<uint8_t>((hours + m_hour) % 24);
		}
		else
		{
			m_hour = static_cast<uint8_t>((hours + m_hour % 24) + 24);
		}
	}
	SEMF_INFO("addHours to %u", m_hour);
}

uint32_t Time::secondsSinceStartOfDay() const
{
	return 3600 * hour() + 60 * minute() + second();
}

uint32_t Time::millisecondsSinceStartOfDay() const
{
	return secondsSinceStartOfDay() * 1000 + millisecond();
}

int32_t Time::timeSpan(const Time& subtract) const
{
	return static_cast<int32_t>(millisecondsSinceStartOfDay()) - static_cast<int32_t>(subtract.millisecondsSinceStartOfDay());
}

Time& Time::operator=(const Time& timeToSet)
{
	setTime(timeToSet.millisecond(), timeToSet.second(), timeToSet.minute(), timeToSet.hour());
	return *this;
}

bool Time::operator==(const Time& timeToCompareWith) const
{
	if (hour() == timeToCompareWith.hour() && minute() == timeToCompareWith.minute() && second() == timeToCompareWith.second() &&
		millisecond() == timeToCompareWith.millisecond())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Time::operator!=(const Time& timeToCompareWith) const
{
	return !(*this == timeToCompareWith);
}

bool Time::operator<(const Time& timeToCompareWith) const
{
	if (hour() > timeToCompareWith.hour() || (hour() == timeToCompareWith.hour() && minute() > timeToCompareWith.minute()) ||
		(hour() == timeToCompareWith.hour() && minute() == timeToCompareWith.minute() && second() > timeToCompareWith.second()) ||
		(hour() == timeToCompareWith.hour() && minute() == timeToCompareWith.minute() && second() == timeToCompareWith.second() &&
		 millisecond() >= timeToCompareWith.millisecond()))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Time::operator<=(const Time& timeToCompareWith) const
{
	if (*this < timeToCompareWith || *this == timeToCompareWith)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Time::operator>(const Time& timeToCompareWith) const
{
	return !(*this <= timeToCompareWith);
}

bool Time::operator>=(const Time& timeToCompareWith) const
{
	return !(*this < timeToCompareWith);
}

bool Time::isValid(uint16_t millisecond, uint8_t second, uint8_t minute, uint8_t hour)
{
	return (millisecond < 1000 && second < 60 && minute < 60 && hour < 24) ? true : false;
}
} /* namespace semf */
