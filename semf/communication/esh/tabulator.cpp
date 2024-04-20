/**
 * @file tabulator.cpp
 * @author jh
 * @date 11.03.2024
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved.
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/communication/esh/tabulator.h>
#include <algorithm>

namespace semf::esh
{
Tabulator::Tabulator(Printer& printer, const LinkedList<Command>& commands, char lineBuffer[], size_t lineBufferSize, std::string_view prompt)
: m_printer(printer),
  m_commands(commands),
  m_lineBuffer(lineBuffer),
  m_lineBufferSize(lineBufferSize),
  kPrompt(prompt)
{
}

void Tabulator::start(size_t charCount)
{
	if (m_busy)
	{
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Start_IsBusy)));
		return;
	}

	m_busy = true;
	m_charCount = charCount;
	m_matchCount = 0;
	m_current = m_commands.cbegin();
	searchMatches();
}

bool Tabulator::isBusy() const
{
	return m_busy;
}

bool Tabulator::prefixMatch(std::string_view sub, std::string_view str) const
{
	if (sub.size() > str.size())
		return false;
	str.remove_suffix(str.size() - sub.size());
	return sub == str;
}

void Tabulator::searchMatches()
{
	for (; m_current != m_commands.cend(); m_current++)
	{
		if (prefixMatch(std::string_view(m_lineBuffer, m_charCount), m_current->name()))
		{
			m_matchCount++;
			m_lastMatch = m_current;
			m_printer.dataWritten.connect(m_printCommandNameSlot);
			m_printer.print("\r\n");
			return;
		}
	}

	if (m_matchCount == 1)
	{
		m_charCount = m_lastMatch->name().size();
		std::copy(m_lastMatch->name().begin(), m_lastMatch->name().end(), m_lineBuffer);
		m_lineBuffer[m_charCount] = 0;
	}

	if (m_matchCount != 0)
	{
		m_printer.dataWritten.clear();
		m_printer.dataWritten.connect(m_updatePromptSlot);
		m_printer.print("\r\n");
	}
	else
	{
		onDone();
	}
}

void Tabulator::printCommandName()
{
	m_printer.dataWritten.disconnect(m_printCommandNameSlot);
	m_printer.dataWritten.connect(m_continueSearchSlot);
	m_printer.print(m_current->name());
}

void Tabulator::continueSearch()
{
	m_printer.dataWritten.disconnect(m_continueSearchSlot);
	m_current++;
	searchMatches();
}

void Tabulator::updatePrompt()
{
	m_printer.dataWritten.clear();
	m_printer.dataWritten.connect(m_printCommandSlot);
	m_printer.print(kPrompt);
}

void Tabulator::printCommand()
{
	m_printer.dataWritten.clear();
	m_printer.dataWritten.connect(m_onDoneSlot);
	m_printer.print(m_lineBuffer);
}

void Tabulator::onDone()
{
	m_printer.dataWritten.disconnect(m_onDoneSlot);
	m_busy = false;
	done(m_charCount);
}
}  // namespace semf::esh
