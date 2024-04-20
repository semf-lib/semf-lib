/**
 * @file debounceddigitalinpolling.cpp
 * @date 22.03.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/input/digitalin/debounceddigitalinpolling.h>
#include <semf/utils/core/debug.h>

namespace semf
{
DebouncedDigitalInPolling::DebouncedDigitalInPolling(Gpio& gpio, uint32_t debounceLowTime, uint32_t debounceHighTime, bool inverted)
: DigitalInPolling(gpio, inverted),
  m_debounceLowTime(debounceLowTime),
  m_debounceHighTime(debounceHighTime)
{
}

DebouncedDigitalInPolling::DebouncedDigitalInPolling(Gpio& gpio, app::TimeBase& timeBase, uint32_t debounceLowTime, uint32_t debounceHighTime, bool inverted)
: DigitalInPolling(gpio, timeBase, inverted),
  m_debounceLowTime(debounceLowTime),
  m_debounceHighTime(debounceHighTime)
{
}

void DebouncedDigitalInPolling::setDebounceHighTime(uint32_t time)
{
	SEMF_INFO("set high time to %u", time);
	m_debounceHighTime = time;
}

void DebouncedDigitalInPolling::setDebounceLowTime(uint32_t time)
{
	SEMF_INFO("set low time to %u", time);
	m_debounceLowTime = time;
}

void DebouncedDigitalInPolling::tick()
{
	app::DigitalIn::State oldState = state();
	app::DigitalIn::State newState = DigitalIn::state();
	if (oldState == app::DigitalIn::State::Low && newState == app::DigitalIn::State::High)
	{
		m_lowTime = 0;
		++m_highTime;
		if (m_highTime >= m_debounceHighTime)
		{
			setState(app::DigitalIn::State::High);
			SEMF_INFO("changed to high");
			changedToHigh();
		}
	}
	else if (oldState == app::DigitalIn::State::High && newState == app::DigitalIn::State::Low)
	{
		m_highTime = 0;
		++m_lowTime;
		if (m_lowTime >= m_debounceLowTime)
		{
			setState(app::DigitalIn::State::Low);
			SEMF_INFO("changed to low");
			changedToLow();
		}
	}
	else
	{
		m_lowTime = 0;
		m_highTime = 0;
	}
}
} /* namespace semf */
