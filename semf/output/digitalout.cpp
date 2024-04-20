/**
 * @file digitalout.cpp
 * @date 15.03.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/output/digitalout.h>
#include <semf/utils/core/debug.h>

namespace semf
{
DigitalOut::DigitalOut(Gpio& gpio, bool inverted)
: m_gpio(gpio),
  m_inverted(inverted)
{
}

void DigitalOut::setInverted(bool inverted)
{
	SEMF_INFO("set inverted to %d", inverted);
	m_inverted = inverted;
}

void DigitalOut::set(State state)
{
	if (state == Low)
	{
		if (!m_inverted)
		{
			SEMF_INFO("reset gpio, State is LOW");
			m_gpio.reset();
		}
		else
		{
			SEMF_INFO("set gpio, State is LOW");
			m_gpio.set();
		}
	}
	else
	{
		if (!m_inverted)
		{
			SEMF_INFO("set gpio, State is HIGH");
			m_gpio.set();
		}
		else
		{
			SEMF_INFO("reset gpio, State is HIGH");
			m_gpio.reset();
		}
	}
}

void DigitalOut::reset()
{
	set(Low);
}

void DigitalOut::toggle()
{
	SEMF_INFO("toggle");
	set(static_cast<State>(!m_gpio.state()));
}

app::DigitalOut::State DigitalOut::state() const
{
	return static_cast<State>(m_gpio.state());
}
} /* namespace semf */
