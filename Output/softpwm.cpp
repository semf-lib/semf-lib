/**
 * @file softpwm.cpp
 *
 * @date 30.09.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <Output/softpwm.h>
#include <Core/debug.h>

namespace semf
{
SoftPwm::SoftPwm(Gpio& output, TimeBase& timebase, bool inverted)
	:m_out(output, inverted),
	 m_pwmCycleTimer(timebase, 1, false)
{

}

void SoftPwm::setMaxValue(unsigned int maxValue)
{
	SEMF_INFO("set max value to %u", maxValue);
	m_maxValue = maxValue;
}

unsigned int SoftPwm::maxValue() const
{
	return m_maxValue;
}

void SoftPwm::set(unsigned int value, bool inverted)
{
	SEMF_INFO("set value to %u and inverted to %d", value, inverted);
	m_pwmValue = value;
	m_out.setInverted(inverted);
	if (!m_pwmCycleTimer.isRunning())
	{
		startOnTime();
	}
}

unsigned int SoftPwm::value() const
{
	return m_pwmValue;
}

void SoftPwm::start()
{
	SEMF_INFO("start");
	startOnTime();
}

void SoftPwm::stop()
{
	SEMF_INFO("stop");
	m_pwmCycleTimer.stop();
	m_out.reset();
}

bool SoftPwm::isEnabled() const
{
	return m_pwmCycleTimer.isRunning();
}

void SoftPwm::startOnTime()
{
	if (checkForConstantSetting())
		return;

	m_pwmCycleTimer.timeout.reconnect(this, &SoftPwm::startOffTime);
	m_pwmCycleTimer.setInterval(m_pwmValue);
	m_out.set();
	m_pwmCycleTimer.start();
}

void SoftPwm::startOffTime()
{
	if (checkForConstantSetting())
		return;

	m_pwmCycleTimer.timeout.reconnect(this, &SoftPwm::startOnTime);
	m_pwmCycleTimer.setInterval(m_maxValue - m_pwmValue);
	m_out.reset();
	m_pwmCycleTimer.start();
}

bool SoftPwm::checkForConstantSetting()
{
	bool constant = false;
	// Constant off
	if (m_pwmValue == 0)
	{
		m_pwmCycleTimer.stop();
		m_out.reset();
		constant = true;
	}
	// Constant on
	else if (m_pwmValue == m_maxValue)
	{
		m_pwmCycleTimer.stop();
		m_out.set();
		constant = true;
	}
	return constant;
}

} /* namespace semf */
