/**
 * @file pwmout.cpp
 *
 * @date 15.03.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <Output/pwmout.h>
#include <Core/debug.h>

namespace semf
{
PwmOut::PwmOut(Pwm& pwm)
	:m_pwm(pwm)
{

}

void PwmOut::setPwm(Pwm& pwm)
{
	SEMF_INFO("new pwm set");
	m_pwm = pwm;
}

void PwmOut::setEnable(bool enable)
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

bool PwmOut::isEnabled() const
{
	return m_pwm.isEnabled();
}

void PwmOut::start()
{
	setEnable(true);
}

void PwmOut::stop()
{
	setEnable(false);
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

	// This distinction is necessary to avoid an overflow.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
	if (m_pwm.maxValue() < (INTMAX_MAX/1000))
	{
		uintmax_t newValue = static_cast<uintmax_t>(m_pwm.maxValue())*static_cast<uintmax_t>(pwm);
		newValue = newValue / 1000;
		setValue(newValue);
	}
	else
	{
		setValue((static_cast<uintmax_t>(m_pwm.maxValue())/static_cast<uintmax_t>(1000))*static_cast<uintmax_t>(pwm));
	}
#pragma GCC diagnostic pop
}

uint16_t PwmOut::permille() const
{
	// This distinction is necessary to avoid an overflow
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
	if (m_pwm.value() < INTMAX_MAX/1000)
	{
		uintmax_t newValue = (static_cast<uintmax_t>(m_pwm.value())*static_cast<uintmax_t>(1000));
		newValue = static_cast<intmax_t>(newValue) / static_cast<intmax_t>(m_pwm.maxValue());
		return newValue;
	}
	else
	{
		return((static_cast<uintmax_t>(m_pwm.value())/static_cast<uintmax_t>(m_pwm.maxValue()))
				*static_cast<uintmax_t>(1000));
	}
#pragma GCC diagnostic pop
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
