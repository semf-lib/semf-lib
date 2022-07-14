/**
 * @file debounceddigitalinpolling.cpp
 *
 * @date 22.03.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <Input/DigitalIn/debounceddigitalinpolling.h>
#include <Core/debug.h>

namespace semf
{
DebouncedDigitalInPolling::DebouncedDigitalInPolling(Gpio& gpio, uint32_t debounceLowTime, uint32_t debounceHighTime, bool inverted)
	:DigitalInPolling(gpio, inverted),
	 m_debounceLowTime(debounceLowTime),
	 m_debounceHighTime(debounceHighTime)
{

}

DebouncedDigitalInPolling::DebouncedDigitalInPolling(Gpio& gpio, TimeBase& timeBase, uint32_t debounceLowTime, uint32_t debounceHighTime, bool inverted)
	:DigitalInPolling(gpio, timeBase, inverted),
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
	State oldState = state();
	State newState = DigitalIn::state();
	if (oldState == State::Low && newState == State::High)
	{
		m_lowTime = 0;
		++m_highTime;
		if (m_highTime >= m_debounceHighTime)
		{
			setState(State::High);
			SEMF_INFO("changed to high");
			changedToHigh();
		}
	}
	else if (oldState == State::High && newState == State::Low)
	{
		m_highTime = 0;
		++m_lowTime;
		if (m_lowTime >= m_debounceLowTime)
		{
			setState(State::Low);
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
