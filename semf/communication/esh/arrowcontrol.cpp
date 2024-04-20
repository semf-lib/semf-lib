/**
 * @file arrowcontrol.cpp
 * @author jh
 * @date 11.03.2024
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved.
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/communication/esh/arrowcontrol.h>

namespace semf::esh
{
ArrowControl::ArrowControl(Printer& printer, const int& count, bool echo)
: m_printer(printer),
  m_count(count),
  m_echo(echo)
{
}

void ArrowControl::start()
{
	if (m_busy)
	{
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Start_IsBusy)));
		return;
	}
	m_busy = true;

	m_printer.dataAvailable.connect(m_onSecondCharSlot);
	m_printer.readCharacter();
}

void ArrowControl::onSecondChar(char c)
{
	m_printer.dataAvailable.disconnect(m_onSecondCharSlot);
	if (c != '[')
	{
		m_busy = false;
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::OnSecondChar_UnexpectedChar)));
		return;
	}

	m_printer.dataAvailable.connect(m_onThirdCharSlot);
	m_printer.readCharacter();
}

void ArrowControl::onThirdChar(char c)
{
	m_printer.dataAvailable.disconnect(m_onThirdCharSlot);
	if (c != 'A' && c != 'B')
	{
		m_busy = false;
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::OnThirdChar_UnexpectedChar)));
		return;
	}

	m_c = c;
	m_printer.dataWritten.connect(m_onCleared);
	m_printer.print("\b \b", m_echo ? m_count : m_count + 4);
}

void ArrowControl::onCleared()
{
	m_printer.dataWritten.disconnect(m_onCleared);
	m_busy = false;
	if (m_c == 'A')
	{
		up();
	}
	else
	{
		down();
	}
}
}  // namespace semf::esh
