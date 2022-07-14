/**
 * @file timebase.cpp
 *
 * @date 26.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <Interfaces/tickreceiver.h>
#include <System/timebase.h>
#include <Core/debug.h>

namespace semf
{
TimeBase::TimeBase(Timer& timer, bool enable)
	:m_enabled(enable)
{
	timer.timeout.reconnect(this, &TimeBase::count);
}

void TimeBase::setTimer(Timer& timer)
{
	timer.timeout.reconnect(this, &TimeBase::count);
}

void TimeBase::add(TickReceiver& tickReceiver)
{
	m_tickReceiverList.pushBack(tickReceiver);
}

void TimeBase::remove(TickReceiver& tickReceiver)
{
	m_tickReceiverList.erase(LinkedList<TickReceiver>::Iterator(&tickReceiver));
}

void TimeBase::setEnabled(bool enable)
{
	m_enabled = enable;
}

void TimeBase::setDisabled(bool disable)
{
	m_enabled = !disable;
}

bool TimeBase::isEnabled() const
{
	return m_enabled;
}

void TimeBase::count()
{
	if (m_enabled && !m_tickReceiverList.empty())
	{
		for (auto &it : m_tickReceiverList)
		{
			SEMF_INFO("tick");
			it.tick();
		}
	}
}
} /* namespace semf */
