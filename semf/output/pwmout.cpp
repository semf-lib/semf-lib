/**
 * @file pwmout.cpp
 * @date 15.03.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/output/pwmout.h>
#include <semf/utils/core/debug.h>

namespace semf
{
PwmOut::PwmOut(Pwm& pwm)
: m_pwm(pwm)
{
}

void PwmOut::enable(bool enable)
{
	SEMF_INFO("set to: %d", enable);
	if (enable && !isEnabled())
	{
		SEMF_INFO("start pwm");
		m_pwm.start();
	}
	else if (!enable && isEnabled())
	{
		SEMF_INFO("stop pwm");
		m_pwm.stop();
	}
}

void PwmOut::disable(bool disable)
{
	enable(!disable);
}

bool PwmOut::isEnabled() const
{
	return m_pwm.isEnabled();
}

void PwmOut::start()
{
	enable();
}

void PwmOut::stop()
{
	disable();
}

void PwmOut::setValue(uint32_t pwm)
{
	if (pwm > m_pwm.maxValue())
	{
		SEMF_INFO("reduced pwm %u to max %u", pwm, m_pwm.maxValue());
		pwm = m_pwm.maxValue();
	}
	SEMF_INFO("set to %u", pwm);
	m_pwm.set(pwm, m_inverted);
}

uint32_t PwmOut::value() const
{
	return m_pwm.value();
}

void PwmOut::setPermille(uint16_t pwm)
{
	if (pwm > 1000)
	{
		SEMF_INFO("reduced pwm %u to max 1000", pwm);
		pwm = 1000;
	}
	setValue(static_cast<uint32_t>(static_cast<uint64_t>(m_pwm.maxValue()) * pwm / 1000));
}

uint16_t PwmOut::permille() const
{
	return static_cast<uint16_t>(m_pwm.value() * 1000ULL / m_pwm.maxValue());
}

void PwmOut::setMaxValue(uint32_t pwm)
{
	// min 1 to prevent divide by zero
	if (pwm == 0)
	{
		SEMF_INFO("set max value to 1 instead of 0");
		m_pwm.setMaxValue(1);
	}
	else
	{
		SEMF_INFO("set max value to %u", pwm);
		m_pwm.setMaxValue(pwm);
	}
}

uint32_t PwmOut::maxValue() const
{
	return m_pwm.maxValue();
}

void PwmOut::setInverted(bool inverted)
{
	SEMF_INFO("set inverted to %d", inverted);
	m_inverted = inverted;
}
} /* namespace semf */
