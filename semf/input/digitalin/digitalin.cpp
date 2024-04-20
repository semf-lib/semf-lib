/**
 * @file digitalin.cpp
 * @date 14.03.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/input/digitalin/digitalin.h>
#include <semf/utils/core/debug.h>

namespace semf
{
DigitalIn::DigitalIn(Gpio& gpio, bool inverted)
: m_gpio(&gpio),
  m_inverted(inverted)
{
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

app::DigitalIn::State DigitalIn::state() const
{
	if (!m_inverted)
		return static_cast<State>(m_gpio->state());
	else
		return static_cast<State>(!m_gpio->state());
}

Gpio& DigitalIn::gpio() const
{
	return *m_gpio;
}
} /* namespace semf */
