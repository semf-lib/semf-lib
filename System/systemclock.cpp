/**
 * @file systemclock.cpp
 *
 * @date 03.09.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <System/systemclock.h>
#include <Core/debug.h>

namespace semf
{
SystemClock::SystemClock(Rtc& rtc)
	:m_rtc(&rtc)
{

}

DateTime& SystemClock::now()
{
	m_dateTime.setDateTime(m_rtc->millisecond(), m_rtc->second(), m_rtc->minute(), m_rtc->hour(),
			m_rtc->day(), static_cast<Date::Month>(m_rtc->month()), m_rtc->year());
	return m_dateTime;
}

void SystemClock::set(DateTime& datetimeToSet)
{
	m_rtc->setSecond(datetimeToSet.second());
	m_rtc->setMinute(datetimeToSet.minute());
	m_rtc->setHour(datetimeToSet.hour());
	m_rtc->setDay(datetimeToSet.day());
	m_rtc->setMonth(datetimeToSet.month());
	m_rtc->setYear(datetimeToSet.year());
	SEMF_INFO("set");
}

SystemClock& SystemClock::operator=(const DateTime& datetimeToSet)
{
	m_rtc->setSecond(datetimeToSet.second());
	m_rtc->setMinute(datetimeToSet.minute());
	m_rtc->setHour(datetimeToSet.hour());
	m_rtc->setDay(datetimeToSet.day());
	m_rtc->setMonth(datetimeToSet.month());
	m_rtc->setYear(datetimeToSet.year());
	m_dateTime = datetimeToSet;
	SEMF_INFO("set");
	return *this;
}

} /* namespace semf */
