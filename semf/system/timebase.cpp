/**
 * @file timebase.cpp
 * @date 26.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/system/timebase.h>
#include <semf/utils/core/debug.h>

namespace semf
{
TimeBase::TimeBase(app::Timer& timer, bool enable)
: m_enabled(enable),
  m_timer(&timer)
{
	m_timer->timeout.connect(m_countSlot);
}

void TimeBase::add(TickReceiver& tickReceiver)
{
	m_tickReceiverList.pushBack(tickReceiver);
}

void TimeBase::remove(TickReceiver& tickReceiver)
{
	m_tickReceiverList.erase(LinkedList<TickReceiver>::Iterator(&tickReceiver));
}

void TimeBase::enable(bool enable)
{
	m_enabled = enable;
}

void TimeBase::disable(bool disable)
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
		for (auto& it : m_tickReceiverList)
		{
			SEMF_INFO("tick");
			it.tick();
		}
	}
}
} /* namespace semf */
