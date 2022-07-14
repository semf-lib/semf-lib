/**
 * @file softwaretimer.cpp
 *
 * @date 20.03.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <System/softwaretimer.h>
#include <System/timebase.h>
#include <Core/debug.h>

namespace semf
{
SoftwareTimer::SoftwareTimer(uint32_t interval, bool start)
	:m_running(start),
	 m_interval(interval)
{

}

SoftwareTimer::SoftwareTimer(TimeBase& timeBase, uint32_t interval, bool start)
:m_running(start),
 m_interval(interval)
{
	timeBase.add(*this);
}

void SoftwareTimer::setInterval(uint32_t interval)
{
	m_interval = interval;
}

uint32_t SoftwareTimer::interval() const
{
	return m_interval;
}

uint32_t SoftwareTimer::counterValue() const
{
	return m_counter;
}

void SoftwareTimer::reset()
{
	m_counter = 0;
}

void SoftwareTimer::tick()
{
	if (!m_running)
		return;

	++m_counter;

	if (m_counter >= m_interval)
	{
		SEMF_INFO("timeout");
		timeout();
		m_counter = 0;
	}
}

void SoftwareTimer::start()
{
	SEMF_INFO("start");
	m_running = true;
}

void SoftwareTimer::stop()
{
	SEMF_INFO("stop");
	m_running = false;
}

bool SoftwareTimer::isRunning() const
{
	return m_running;
}

} /* namespace semf */
