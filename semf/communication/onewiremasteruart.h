/**
 * @file onewiremasteruart.h
 * @date Jul 22, 2024
 * @author vb
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_COMMUNICATION_ONEWIREMASTERUART_H_
#define SEMF_COMMUNICATION_ONEWIREMASTERUART_H_

#include <semf/utils/core/queues/linkedqueue.h>
#include <semf/utils/core/signals/slot.h>
#include <semf/communication/onewiremaster.h>
#include <semf/communication/uarthardware.h>

namespace semf
{
/**
 * @brief \c 1-WireMasterHardware implementation using uart.
 */
class OneWireMasterUart : public OneWireMaster
{
public:
	/**
	 * @brief Constructor.
	 * @param uart The uart hardware.
	 * @param highTimingBaud The baudrate for read/write operations.
	 * @param lowTimingBaud The baudrate for the reset operation.
	 * @note Each UART bit’s time slot is calculated with the inverse of the baud rate (1/baud rate).
	 * The UART’s start bit, as it is always low, can be used in combination with the eight data bits
	 * to develop the appropriate timing. The UART \b must transmit the least significant bit first.
	 * Transmitting a byte of 0xF0 makes data bits 0 to 3 a logic 0 and data bits 4 to 7 a logic 1, which
	 * creates a Reset Low Time of 520.83us at 9600 baud (including the uart start bit).
	 * This implementation uses 0xFF as read pattern, 0x00 as write-0 pattern, 0xFF as write-1 pattern
	 * and 0xF0 as reset pattern. Uart will be configured with no parity, 8 data bits and one stop bit.
	 * Calculation example for Reset Low Time and baudrate 9600: (1 / 9600) * (4 + 1) = 520.83us where 4 data bits
	 * are low adding the uart start bit for a total of 5 consecutive low bits.
	 */
	explicit OneWireMasterUart(UartHardware& uart, uint32_t highTimingBaud = 115200, uint32_t lowTimingBaud = 9600);
	explicit OneWireMasterUart(const OneWireMasterUart& other) = delete;
	virtual ~OneWireMasterUart() = default;
	/**
	 * @brief The timing baudrate for read/write operations. See \ref semf::OneWireMasterUart::OneWireMasterUart()
	 */
	uint32_t highTiming() const;
	/**
	 * @brief The timing baudrate for the reset operation. See \ref semf::OneWireMasterUart::OneWireMasterUart()
	 */
	uint32_t lowTiming() const;
	/**
	 * @brief Set the timing baudrate for read/write operations. See \ref semf::OneWireMasterUart::OneWireMasterUart()
	 * @param baudrate The baudrate to set.
	 */
	void setHighTiming(uint32_t baudrate);
	/**
	 * @brief Set the timing baudrate for the reset operation. See \ref semf::OneWireMasterUart::OneWireMasterUart()
	 * @param baudrate The baudrate to set.
	 */
	void setLowTiming(uint32_t baudrate);

private:
	enum class Operation : uint8_t
	{
		None = 0,
		Write,
		Read,
		Reset
	};

	void readBit() override;
	void writeBit(bool bit) override;
	void reset() override;
	void evaluate();

	static constexpr uint8_t kWrite0Pattern = 0x00;
	static constexpr uint8_t kWrite1Pattern = 0xFF;
	static constexpr uint8_t kReadPattern = 0xFF;
	static constexpr uint8_t kResetPattern = 0xF0;
	static constexpr uint8_t kPresencePattern = 0xE0;

	UartHardware& m_uart;
	uint32_t m_highBaudrate;
	uint32_t m_lowBaudrate;
	Operation m_currentOperation = Operation::None;
	uint8_t m_sampleData = 0x00;

	SEMF_SLOT(m_onDataAvailableSlot, OneWireMasterUart, *this, evaluate);
	SEMF_SLOT(m_onDataWrittenSlot, OneWireMasterUart, *this, evaluate);
	SEMF_SLOT(m_onError, Signal<Error>, error, emitSignal, Error);

	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::OneWireMasterUart;
};
}  // namespace semf

#endif  // SEMF_COMMUNICATION_ONEWIREMASTERUART_H_
