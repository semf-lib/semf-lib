/**
 * @file printer.cpp
 * @author jh
 * @date 11.03.2024
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved.
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/communication/esh/printer.h>

namespace semf::esh
{
Printer::Printer(semf::UartHardware& uart)
: m_uart(uart)
{
	m_uart.error.connect(m_uartErrorSlot);
}

void Printer::print(char character, size_t count)
{
	if (m_busy)
	{
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Print_IsBusy)));
		return;
	}

	m_busy = true;
	m_char = character;
	m_count = count;
	m_uart.dataWritten.clear();
	m_uart.dataWritten.connect(m_onCharWrittenSlot);
	m_uart.write(reinterpret_cast<uint8_t*>(&m_char), 1);
}

void Printer::print(std::string_view text, size_t count)
{
	if (m_busy)
	{
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Print_IsBusy)));
		return;
	}

	if (count == 0 || text.empty())
	{
		dataWritten();
		return;
	}

	m_busy = true;
	m_string = text;
	m_count = count;
	m_uart.dataWritten.clear();
	m_uart.dataWritten.connect(m_onStringWrittenSlot);
	m_uart.write(reinterpret_cast<const uint8_t*>(m_string.data()), m_string.size());
}

void Printer::readCharacter()
{
	m_uart.dataAvailable.connect(m_onDataAvailableSlot);
	m_uart.read(&m_availableChar, sizeof(m_availableChar));
}

bool Printer::isBusy() const
{
	return m_busy;
}

void Printer::onCharWritten()
{
	if (--m_count == 0)
	{
		onDataWritten();
		return;
	}
	m_uart.write(reinterpret_cast<uint8_t*>(&m_char), 1);
}

void Printer::onStringWritten()
{
	if (--m_count == 0)
	{
		onDataWritten();
		return;
	}
	m_uart.write(reinterpret_cast<const uint8_t*>(m_string.data()), m_string.size());
}

void Printer::onDataAvailable()
{
	m_uart.dataAvailable.disconnect(m_onDataAvailableSlot);
	dataAvailable(m_availableChar);
}

void Printer::onDataWritten()
{
	m_uart.dataWritten.disconnect(m_onStringWrittenSlot);
	m_busy = false;
	dataWritten();
}
}  // namespace semf::esh
