/**
 * @file onewiremasteruart.cpp
 * @date Jul 22, 2024
 * @author vb
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/communication/onewiremasteruart.h>

namespace semf
{
OneWireMasterUart::OneWireMasterUart(UartHardware& uart, uint32_t highTimingBaud, uint32_t lowTimingBaud)
: m_uart{uart},
  m_highBaudrate{highTimingBaud},
  m_lowBaudrate{lowTimingBaud}
{
	m_uart.dataWritten.connect(m_onDataWrittenSlot);
	m_uart.dataAvailable.connect(m_onDataAvailableSlot);
	m_uart.error.connect(m_onError);

	m_uart.setBaud(m_highBaudrate);
	m_uart.setFormat(8, Uart::Parity::NoParity, Uart::StopBits::Stopbits_1, Uart::FlowControl::NoFlowControl);
	m_uart.setFrame(CommunicationHardware::Frame::FirstAndLast);
	m_uart.setDirection(Uart::Direction::RxTx);
	m_uart.setWireMode(Uart::WireMode::Simplex);
}

uint32_t OneWireMasterUart::highTiming() const
{
	return m_highBaudrate;
}

uint32_t OneWireMasterUart::lowTiming() const
{
	return m_lowBaudrate;
}

void OneWireMasterUart::setHighTiming(uint32_t baudrate)
{
	m_highBaudrate = baudrate;
}

void OneWireMasterUart::setLowTiming(uint32_t baudrate)
{
	m_lowBaudrate = baudrate;
}

void OneWireMasterUart::readBit()
{
	m_currentOperation = Operation::Read;

	m_uart.setBaud(m_highBaudrate);
	m_uart.read(&m_sampleData, 1);
	m_uart.write(&kReadPattern, 1);
}

void OneWireMasterUart::writeBit(bool bit)
{
	m_currentOperation = Operation::Write;
	m_uart.setBaud(m_highBaudrate);

	uint8_t pattern = 0;
	if (bit)
	{
		pattern = kWrite1Pattern;
	}
	else
	{
		pattern = kWrite0Pattern;
	}

	m_uart.write(&pattern, 1);
}

void OneWireMasterUart::reset()
{
	m_currentOperation = Operation::Reset;

	m_uart.setBaud(m_lowBaudrate);
	m_uart.read(&m_sampleData, 1);
	m_uart.write(&kResetPattern, 1);
}

void OneWireMasterUart::evaluate()
{
	if (m_uart.isBusyReading() || m_uart.isBusyWriting())
		return;

	if (m_currentOperation == Operation::Write)
	{
		bitWritten();
	}
	else if (m_currentOperation == Operation::Read)
	{
		bitRead((m_sampleData & 0x01) != 0);
	}
	else if (m_currentOperation == Operation::Reset)
	{
		resetted(m_sampleData == kPresencePattern);
	}
}
}  // namespace semf
