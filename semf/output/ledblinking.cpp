/**
 * @file ledblinking.cpp
 * @date 30.09.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/output/ledblinking.h>
#include <semf/utils/core/debug.h>
#include <limits>

namespace semf
{
LedBlinking::LedBlinking(Gpio& ledPin, app::TimeBase& timebase, bool inverted)
: m_pwm(ledPin, timebase, inverted),
  m_inverted(inverted)
{
}

void LedBlinking::setOn()
{
	SEMF_INFO("set on");
	m_pwm.setMaxValue(10);
	m_pwm.set(10);
	m_pwm.start();
	m_mode = Mode::On;
}

void LedBlinking::setOff()
{
	SEMF_INFO("set off");
	m_pwm.set(0);
	m_pwm.start();
	m_mode = Mode::Off;
}

void LedBlinking::toggle()
{
	SEMF_INFO("toggle");
	// Led is on, if value is maxValue
	if (m_mode == Mode::On)
		setOff();
	else if (m_mode == Mode::Off)
		setOn();
}

void LedBlinking::setBlinking(unsigned int onOffTime)
{
	setBlinking(onOffTime, onOffTime);
}

void LedBlinking::setBlinking(unsigned int onTime, unsigned int offTime)
{
	SEMF_INFO("set blinking with on time %u and off time %u", onTime, offTime);
	m_pwm.setMaxValue(onTime + offTime);
	m_pwm.set(onTime, m_inverted);
	if (!m_pwm.isEnabled())
	{
		SEMF_INFO("start pwm");
		m_pwm.start();
	}
	m_mode = Mode::Blinking;
}

LedBlinking::Mode LedBlinking::mode() const
{
	return m_mode;
}
} /* namespace semf */
