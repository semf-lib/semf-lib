/**
 * @file history.cpp
 * @author jh
 * @date 12.03.2024
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved.
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef THIRDPARTY_ESH_SHELLHISTORY_CPP_
#define THIRDPARTY_ESH_SHELLHISTORY_CPP_

#include <semf/communication/esh/history.h>
#include <algorithm>

namespace semf::esh
{
History::History(char* historyBuffer, size_t numberOfEntries, size_t lineSize)
: m_history{historyBuffer},
  m_numberOfEntries{numberOfEntries},
  m_lineSize{lineSize}
{
}

void History::insert(std::string_view command)
{
	if (command.empty() || m_history == nullptr)
	{
		return;
	}
	int index = m_savedCommands % m_numberOfEntries;
	std::copy_n(command.begin(), std::min(command.size(), m_lineSize - 1), m_history + index * m_lineSize);
	m_history[index * m_lineSize + std::min(command.size(), m_lineSize - 1)] = 0;
	m_index = ++m_savedCommands;
}

void History::handleArrowUp(char commandBuffer[])
{
	if (!(m_index < static_cast<int32_t>(m_savedCommands - m_numberOfEntries) || m_index == 0 || m_history == nullptr))
	{
		m_index--;
		int index = m_index % m_numberOfEntries;
		if (std::equal(m_history + index * m_lineSize, m_history + index * m_lineSize + m_lineSize, commandBuffer))
			return;
		std::copy(m_history + index * m_lineSize, m_history + index * m_lineSize + m_lineSize, commandBuffer);
	}
	else
	{
		commandBuffer[0] = 0;
	}
}
void History::handleArrowDown(char commandBuffer[])
{
	if (m_index == m_savedCommands || m_history == nullptr)
	{
		commandBuffer[0] = 0;
		return;
	}

	m_index++;
	int index = m_index % m_numberOfEntries;
	std::copy(m_history + index * m_lineSize, m_history + index * m_lineSize + m_lineSize, commandBuffer);
}
}  // namespace semf::esh
#endif /* THIRDPARTY_ESH_SHELLHISTORY_CPP_ */
