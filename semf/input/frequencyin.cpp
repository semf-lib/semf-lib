/**
 * @file frequencyin.cpp
 * @date 15.03.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/input/frequencyin.h>
#include <semf/system/criticalsection.h>
#include <semf/utils/core/debug.h>

namespace semf
{
FrequencyIn::FrequencyIn(InputCapture& inputCapture)
: m_inputCapture(inputCapture)
{
	m_inputCapture.dataAvailable.connect(m_onDataAvailableSlot);
}

void FrequencyIn::start()
{
	SEMF_INFO("start");
	m_inputCapture.start();
}

void FrequencyIn::stop()
{
	SEMF_INFO("stop");
	m_inputCapture.stop();
}

void FrequencyIn::reset()
{
	SEMF_INFO("reset");
	m_lastPeriod = 0;
}

uint32_t FrequencyIn::periodTicks() const
{
	CriticalSection::enter();
	uint32_t lastPeriod = m_lastPeriod;
	CriticalSection::exit();
	return lastPeriod;
}

uint32_t FrequencyIn::periodInMs() const
{
	unsigned int ticksPerSecond = m_inputCapture.ticksPerSecond();
	if (ticksPerSecond == 0)
		return 0;
	else
		return (periodTicks() * 1000) / ticksPerSecond;
}

uint32_t FrequencyIn::frequency() const
{
	uint32_t ticks = periodTicks();
	if (ticks == 0)
		return 0;
	return (m_inputCapture.ticksPerSecond() / ticks);
}

void FrequencyIn::onDataAvailable()
{
	CriticalSection::enter();
	uint32_t currentTicks = m_inputCapture.ticks();

	if (m_lastTicks < currentTicks)
	{
		m_lastPeriod = currentTicks - m_lastTicks;
	}
	else
	{
		m_lastPeriod = (m_inputCapture.maxTicks() - m_lastTicks) + currentTicks;
	}
	m_lastTicks = currentTicks;
	CriticalSection::exit();
	// call a signal
	dataAvailable();
}
} /* namespace semf */
