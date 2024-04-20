/**
 * @file digitalinpolling.cpp
 * @date 22.03.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/input/digitalin/digitalinpolling.h>
#include <semf/utils/core/debug.h>

namespace semf
{
DigitalInPolling::DigitalInPolling(Gpio& gpio, bool inverted)
: DigitalIn(gpio, inverted)
{
	m_state = DigitalIn::state();
}

DigitalInPolling::DigitalInPolling(Gpio& gpio, app::TimeBase& timeBase, bool inverted)
: DigitalIn(gpio, inverted)
{
	timeBase.add(*this);
	m_state = DigitalIn::state();
}

void DigitalInPolling::tick()
{
	app::DigitalIn::State newState = DigitalIn::state();
	if (state() == app::DigitalIn::State::Low && newState == app::DigitalIn::State::High)
	{
		setState(State::High);
		SEMF_INFO("changed to high");
		changedToHigh();
	}
	else if (state() == State::High && newState == app::DigitalIn::State::Low)
	{
		setState(app::DigitalIn::State::Low);
		SEMF_INFO("changed to low");
		changedToLow();
	}
}

app::DigitalIn::State DigitalInPolling::state() const
{
	return m_state;
}

void DigitalInPolling::setState(app::DigitalIn::State state)
{
	SEMF_INFO("set state to %s", state == State::High ? "high" : "low");
	m_state = state;
}
} /* namespace semf */
