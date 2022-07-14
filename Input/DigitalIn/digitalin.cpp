/**
 * @file digitalin.cpp
 *
 * @date 14.03.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <Input/DigitalIn/digitalin.h>
#include <Core/debug.h>

namespace semf
{
DigitalIn::DigitalIn(Gpio& gpio, bool inverted)
	:m_gpio(gpio),
	 m_inverted(inverted)
{

}

void DigitalIn::setGpio(Gpio& gpio)
{
	SEMF_INFO("set new gpio and not inverted");
	m_gpio = gpio;
	m_inverted = false;
}

bool DigitalIn::isInverted() const
{
	return m_inverted;
}

void DigitalIn::setInverted(bool inverted)
{
	SEMF_INFO("set inverted: %d", inverted);
	m_inverted = inverted;
}

DigitalIn::State DigitalIn::state() const
{
	if (!m_inverted)
		return static_cast<State>(m_gpio.state());
	else
		return static_cast<State>(!m_gpio.state());
}

Gpio& DigitalIn::gpio() const
{
	return m_gpio;
}

} /* namespace semf */
