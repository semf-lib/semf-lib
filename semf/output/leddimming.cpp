/**
 * @file leddimming.cpp
 * @date 10.10.2018
 * @author aa
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/output/leddimming.h>
#include <semf/utils/core/debug.h>
#define _USE_MATH_DEFINES
#include <cmath>

namespace semf
{
LedDimming::LedDimming(PwmOut& dimmPwmOut, app::TimeBase& timebase)
: m_dimmPwm(dimmPwmOut),
  m_timer(timebase, 1, false)
{
}

void LedDimming::setBrightness(uint16_t maxBrightness)
{
	if (maxBrightness > 1000)
	{
		SEMF_INFO("reduce brightness %u to max brightness 1000", maxBrightness);
		maxBrightness = 1000;
	}
	m_maxBrightness = maxBrightness;
	SEMF_INFO("set pwm permille to %u", m_maxBrightness);
	m_dimmPwm.setPermille(m_maxBrightness);
}

void LedDimming::setBrightness(uint16_t minBrightness, uint16_t maxBrightness)
{
	if (maxBrightness > 1000)
	{
		SEMF_INFO("reduce brightness %u to max brightness 1000", maxBrightness);
		maxBrightness = 1000;
	}
	if (minBrightness > maxBrightness)
	{
		SEMF_INFO("reduce minBrightness %u to maxBrightness %u", minBrightness, maxBrightness);
		minBrightness = maxBrightness;
	}
	m_maxBrightness = maxBrightness;
	m_minBrightness = minBrightness;
	SEMF_INFO("set pwm permille to %u", m_maxBrightness);
	m_dimmPwm.setPermille(m_maxBrightness);
}

void LedDimming::setOn()
{
	SEMF_INFO("set on");
	m_mode = Mode::On;
	m_dimmPwm.start();
	m_timer.stop();
}

void LedDimming::setOff()
{
	SEMF_INFO("set off");
	m_mode = Mode::Off;
	m_dimmPwm.stop();
	m_timer.stop();
}

void LedDimming::toggle()
{
	SEMF_INFO("toggle");
	if (m_mode == Mode::On)
		setOff();
	else if (m_mode == Mode::Off)
		setOn();
}

void LedDimming::setBlinking(unsigned int onOffTime)
{
	setBlinking(onOffTime, onOffTime);
}

void LedDimming::setBlinking(unsigned int onTime, unsigned int offTime)
{
	SEMF_INFO("set blinking with on time %u and off time %u", onTime, offTime);
	m_dimmPwm.setPermille(m_maxBrightness);
	m_blinkOnTicks = onTime;
	m_blinkOffTicks = offTime;

	if (m_mode != Mode::Blinking)
	{
		m_mode = Mode::Blinking;
		m_dimmPwm.start();
		m_timer.reset();
		startOnTimeInBlinkingMode();
	}
}

void LedDimming::setSine(unsigned int periodInTimeBaseTicks, unsigned int startIndexInTimeBaseTicks)
{
	setSine(periodInTimeBaseTicks);
	if (startIndexInTimeBaseTicks < periodInTimeBaseTicks)
	{
		SEMF_INFO("start index %u", startIndexInTimeBaseTicks);
		m_waveformStepIndex = startIndexInTimeBaseTicks;
	}
}

void LedDimming::setSine(unsigned int periodInTimeBaseTicks)
{
	SEMF_INFO("set sine with period", periodInTimeBaseTicks);
	m_waveformPeriodTicks = periodInTimeBaseTicks;
	if (m_mode != Mode::Sine)
	{
		m_mode = Mode::Sine;
		m_waveformStepIndex = 0;
		m_timer.stop();
		m_timer.reset();
		m_timer.setInterval(1);
		m_timer.timeout.clear();
		m_timer.timeout.connect(m_nextSineValueSlot);
		m_timer.start();
		m_dimmPwm.enable();
	}
}

void LedDimming::setTriangle(unsigned int periodInTimeBaseTicks, unsigned int startIndexInTimeBaseTicks)
{
	setTriangle(periodInTimeBaseTicks);
	if (startIndexInTimeBaseTicks < periodInTimeBaseTicks)
	{
		SEMF_INFO("start index %u", startIndexInTimeBaseTicks);
		m_waveformStepIndex = startIndexInTimeBaseTicks;
	}
}

void LedDimming::setTriangle(unsigned int periodInTimeBaseTicks)
{
	SEMF_INFO("set triangle with period", periodInTimeBaseTicks);
	m_waveformPeriodTicks = periodInTimeBaseTicks;
	if (m_mode != Mode::Triangule)
	{
		m_mode = Mode::Triangule;
		m_waveformStepIndex = 0;
		m_timer.stop();
		m_timer.reset();
		m_timer.setInterval(1);
		m_timer.timeout.clear();
		m_timer.timeout.connect(m_nextTriangleValueSlot);
		m_timer.start();
		m_dimmPwm.enable();
	}
}

LedDimming::Mode LedDimming::mode() const
{
	return m_mode;
}

void LedDimming::startOnTimeInBlinkingMode()
{
	if (checkForConstantSettingInBlinkingMode())
		return;

	m_timer.timeout.clear();
	m_timer.timeout.connect(m_startOffTimeInBlinkingModeSlot);
	m_timer.setInterval(m_blinkOnTicks);
	m_dimmPwm.start();
	m_timer.start();
}

void LedDimming::startOffTimeInBlinkingMode()
{
	if (checkForConstantSettingInBlinkingMode())
		return;

	m_timer.timeout.clear();
	m_timer.timeout.connect(m_startOnTimeInBlinkingModeSlot);
	m_timer.setInterval(m_blinkOffTicks);
	m_dimmPwm.stop();
	m_timer.start();
}

bool LedDimming::checkForConstantSettingInBlinkingMode()
{
	bool constant = false;
	// Constant off
	if (m_blinkOnTicks == 0)
	{
		setOn();
		constant = true;
	}
	// Constant on
	else if (m_blinkOffTicks == 0)
	{
		setOff();
		constant = true;
	}
	return constant;
}

void LedDimming::nextSineValue()
{
	double tmpBrightness = ceil(500.0 * sin(2.0 * M_PI * m_waveformStepIndex / m_waveformPeriodTicks) + 500.0);
	m_dimmPwm.setPermille(m_minBrightness + static_cast<uint16_t>((m_maxBrightness - m_minBrightness) / 1000 * tmpBrightness));

	m_waveformStepIndex++;
	m_waveformStepIndex %= m_waveformPeriodTicks;
}

void LedDimming::nextTriangleValue()
{
	double tmpBrightness;
	unsigned int halfperiod = m_waveformPeriodTicks >> 1;
	// increasing
	if (m_waveformStepIndex == 0)
		tmpBrightness = 0;
	else if (m_waveformStepIndex < halfperiod)
		tmpBrightness = 2.0 * 1000 * m_waveformStepIndex / m_waveformPeriodTicks;
	// decreasing
	else
		tmpBrightness = 2.0 * 1000 * (m_waveformPeriodTicks - m_waveformStepIndex) / m_waveformPeriodTicks;
	m_dimmPwm.setPermille(m_minBrightness + static_cast<uint16_t>((m_maxBrightness - m_minBrightness) / 1000 * tmpBrightness));

	m_waveformStepIndex++;
	m_waveformStepIndex %= m_waveformPeriodTicks;
}
} /* namespace semf */
