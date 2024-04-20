/**
 * @file shell.cpp
 * @author jh
 * @date 11.03.2024
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved.
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/communication/esh/shell.h>
#include <semf/communication/uarthardware.h>
#include <algorithm>
#include <string>

namespace semf::esh
{
Shell::Shell(UartHardware& uart, const Config& config)
: m_config{config},
  m_history(config.historyBuffer, config.numberOfEntries, config.lineBufferSize),
  m_printer(uart),
  m_tab(m_printer, m_commands, config.lineBuffer, config.lineBufferSize, config.prompt),
  m_processor(m_commands, m_printer, config.lineBuffer, config.lineBufferSize, config.argvBuffer, config.argvBufferSize),
  m_arrow(m_printer, m_count, config.echo)
{
	m_printer.error.connect(m_onErrorSlot);
	m_arrow.up.connect(m_onArrowUpSlot);
	m_arrow.down.connect(m_onArrowDownSlot);
	m_arrow.error.connect(m_onErrorSlot);
	m_tab.error.connect(m_onErrorSlot);
	m_tab.done.connect(m_onTabDoneSlot);
	m_processor.error.connect(m_onErrorSlot);
}

void Shell::addCommand(Command& cmd)
{
	m_commands.pushBack(cmd);
}

void Shell::start()
{
	m_printer.dataWritten.disconnect(m_restartShell);
	m_printer.dataWritten.connect(m_readNextCharSlot);
	m_printer.print(m_config.prompt);
}

void Shell::loop()
{
	m_processor.execLoadedCommand();
}

void Shell::onCharAvailable(char character)
{
	m_printer.dataAvailable.disconnect(m_onCharAvailableSlot);
	switch (character)
	{
		case '\r':
		case '\n':
			m_config.lineBuffer[m_count] = 0;
			m_history.insert(m_config.lineBuffer);
			m_processor.done.connect(m_onExecutedSlot);
			m_processor.loadCommand();
			break;
		case kDelete:
		case '\b':
			if (m_count == 0)
			{
				readNextCharacter();
				return;
			}
			m_count--;
			m_config.lineBuffer[m_count] = 0;

			m_printer.dataWritten.connect(m_readNextCharSlot);
			m_printer.print("\b \b", m_config.echo ? 1 : 3);

			break;
		case kEscape:
			m_arrow.start();
			break;
		case '\t':
			m_tab.start(m_count);
			break;
		default:
			m_config.lineBuffer[m_count++] = character;
			if (m_config.echo && character != kDelete && character != '\b')
			{
				m_printer.dataWritten.connect(m_readNextCharSlot);
				m_printer.print(character, 1);
			}
			else
			{
				readNextCharacter();
			}
			break;
	}
}
void Shell::onTabDone(size_t charCount)
{
	if (charCount != 0)
	{
		m_count = charCount;
	}
	readNextCharacter();
}

void Shell::onExecuted()
{
	m_processor.done.disconnect(m_onExecutedSlot);
	m_printer.dataWritten.connect(m_restartShell);
	std::fill(m_config.lineBuffer, m_config.lineBuffer + m_config.lineBufferSize, 0);
	m_count = 0;
	m_printer.print("\r\n");
}

void Shell::onArrowUp()
{
	m_history.handleArrowUp(m_config.lineBuffer);
	updateStateAndPrint();
}
void Shell::onArrowDown()
{
	m_history.handleArrowDown(m_config.lineBuffer);
	updateStateAndPrint();
}

void Shell::updateStateAndPrint()
{
	m_count = std::char_traits<char>::length(m_config.lineBuffer);
	m_printer.dataWritten.connect(m_readNextCharSlot);
	m_printer.print(m_config.lineBuffer);
}

void Shell::onError(Error error)
{
	if (error.classId() == Error::ClassID::ArrowControl && (error.errorCode() == static_cast<uint8_t>(ArrowControl::ErrorCode::OnSecondChar_UnexpectedChar) ||
															error.errorCode() == static_cast<uint8_t>(ArrowControl::ErrorCode::OnThirdChar_UnexpectedChar)))
	{
		m_printer.dataAvailable.connect(m_onCharAvailableSlot);
		m_printer.readCharacter();
	}
	else
	{
		this->error(error);
	}
}

void Shell::readNextCharacter()
{
	m_printer.dataWritten.disconnect(m_readNextCharSlot);
	m_printer.dataAvailable.connect(m_onCharAvailableSlot);
	m_printer.readCharacter();
}
}  // namespace semf::esh
