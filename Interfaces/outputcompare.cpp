/**
 * @file	outputcompare.cpp
 *
 * @date	12.03.2020
 * @author	aa
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <Interfaces/outputcompare.h>
#include <Core/debug.h>

namespace semf
{
void OutputCompare::setFrequency(uint32_t frequency /*hz*/)
{
	SEMF_INFO("set frequency %u", frequency);
	if (m_isInitialized)
	{
		// frequency correction
		if (frequency >= timerInputFrequency())
		{
			frequency = timerInputFrequency();
		}
		else if (frequency == 0)
		{
			SEMF_ERROR("set invalid frequency %u", frequency);
			frequency = 0;
		}

		// core of the function
		if (m_outputFrequency != frequency)
		{
			m_outputFrequency = frequency;
			setTimerConfiguration();
		}
	}
	else
	{
		initHardware();
		m_isInitialized = true;
		setFrequency(frequency);
	}
}

void OutputCompare::start()
{
	SEMF_INFO("start");
	if (m_isInitialized)
	{
		if (frequency() == 0)
		{
			stop();
		}
		else if (!m_enabled)
		{
			m_enabled = true;
			startHardware();
		}
	}
	else
	{
		initHardware();
		m_isInitialized = true;
		start();
	}
}

void OutputCompare::stop()
{
	SEMF_INFO("stop");
	if (m_isInitialized)
	{
		if (m_enabled)
		{
			m_enabled = false;
			stopHardware();
		}
	}
	else
	{
		initHardware();
		m_isInitialized = true;
		stop();
	}
}

uint32_t OutputCompare::frequency() const
{
	return m_outputFrequency;
}

bool OutputCompare::isEnabled() const
{
	return m_enabled;
}
}  /* namespace semf */
