/**
 * @file processor.cpp
 * @author jh
 * @date 11.03.2024
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved.
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/communication/esh/processor.h>
#include <algorithm>
#include <string>

namespace semf::esh
{
Processor::Processor(const LinkedList<Command>& commands, Printer& printer, char lineBuffer[], size_t lineBufferSize, char** argvBuffer, size_t argvBufferSize)
: m_commands(commands),
  m_printer(printer),
  m_lineBuffer{lineBuffer},
  m_lineBufferSize{lineBufferSize},
  m_argv{argvBuffer},
  m_argvSize{argvBufferSize}
{
	std::fill_n(m_argv, m_argvSize, nullptr);
}

void Processor::loadCommand()
{
	if (m_busy)
	{
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::LoadCommand_IsBusy)));
		return;
	}
	if (m_lineBuffer[0] == '\0')
	{
		done();
		return;
	}

	m_busy = true;
	parseLine();

	for (const Command& command : m_commands)
	{
		if (command.name() == m_argv[0])
		{
			m_currentCommand = &command;
			return;
		}
	}

	m_lastExitStatus = -1;
	printError();
}

void Processor::execLoadedCommand()
{
	if (m_currentCommand == nullptr)
		return;

	m_currentCommand->command(m_argc, m_argv, m_printer, m_lastExitStatus);
	std::fill_n(m_argv, m_argvSize, nullptr);
	m_currentCommand = nullptr;
	onDone();
}

void Processor::parseLine()
{
	int pos = 0;
	int length = std::char_traits<char>::length(m_lineBuffer);
	m_argc = 0;

	while (pos <= length)
	{
		if (m_lineBuffer[pos] != '\t' && m_lineBuffer[pos] != ' ' && m_lineBuffer[pos] != '\0')
			m_argv[m_argc++] = &m_lineBuffer[pos];

		for (; m_lineBuffer[pos] != '\t' && m_lineBuffer[pos] != ' ' && m_lineBuffer[pos] != '\0'; pos++)
		{
		}

		if (m_lineBuffer[pos] == '\t' || m_lineBuffer[pos] == ' ')
			m_lineBuffer[pos] = '\0';

		pos++;
	}
}

void Processor::printError()
{
	m_printer.dataWritten.connect(m_onQuotationMarkPrintedSlot);
	m_printer.print("\r\n\"");
}

void Processor::onQuotationMarkPrinted()
{
	m_printer.dataWritten.disconnect(m_onQuotationMarkPrintedSlot);
	m_printer.dataWritten.connect(m_onNamePrintedSlot);
	m_printer.print(m_argv[0]);
}
void Processor::onNamePrinted()
{
	m_printer.dataWritten.disconnect(m_onNamePrintedSlot);
	m_printer.dataWritten.connect(m_onDoneSlot);
	m_printer.print("\": command not found.\r\n");
}
void Processor::onDone()
{
	m_printer.dataWritten.disconnect(m_onDoneSlot);
	m_busy = false;
	done();
}
}  // namespace semf::esh
