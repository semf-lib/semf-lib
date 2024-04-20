/**
 * @file printer.h
 * @author jh
 * @date 11.03.2024
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved.
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_COMMUNICATION_ESH_PRINTER_H_
#define SEMF_COMMUNICATION_ESH_PRINTER_H_

#include <semf/communication/uarthardware.h>
#include <semf/utils/core/error.h>
#include <semf/utils/core/signals/signal.h>
#include <semf/utils/core/signals/slot.h>
#include <string_view>

namespace semf::esh
{
/**
 * @brief Class for string related UART communication. Using this class outside of an esh-context can make sense too and is encouraged.
 */
class Printer
{
public:
	/**
	 * @brief Error codes for this class. Error ID identify a unique error() / onError call (excluding transferring).
	 */
	enum class ErrorCode : uint8_t
	{
		Print_IsBusy = 0
	};
	/**
	 * @brief Constructor.
	 * @param uart UART to use.
	 */
	explicit Printer(semf::UartHardware& uart);
	Printer(const Printer& other) = delete;
	virtual ~Printer() = default;
	/**
	 * @brief Prints a single character \c count times.
	 * @param character Character to print.
	 * @param count Amout of print cycles.
	 * @throws Print_IsBusy If object is busy.
	 */
	void print(char character, size_t count);
	/**
	 * @brief Prints a string \c count times.
	 * @param text String to print.
	 * @param count Amount of print cycles.
	 * @throws Print_IsBusy If object is busy.
	 */
	void print(std::string_view text, size_t count = 1);
	/**
	 * @brief Triggers a read cyle for reading a single character.
	 */
	void readCharacter();
	/**
	 * @brief Indicates wether the printer is busy.
	 * @return \c true if busy.
	 */
	bool isBusy() const;
	/** Emitted after print finished successfully.*/
	SEMF_SIGNAL(dataWritten);
	/** Emitted after character was received.*/
	SEMF_SIGNAL(dataAvailable, char);
	/** Emitted on error.*/
	SEMF_SIGNAL(error, Error);

private:
	/** Gets called after a singel character has been written.*/
	void onCharWritten();
	/** Gets called after a string has been written.*/
	void onStringWritten();
	/** Gets called after a character has been received.*/
	void onDataAvailable();
	/** Manages overlap between \c onCharWritten and \c onStringWritten .*/
	void onDataWritten();

	/** UART used for communication.*/
	semf::UartHardware& m_uart;
	/** Character to write.*/
	char m_char;
	/** String to write.*/
	std::string_view m_string;
	/** Number of write cycles.*/
	size_t m_count = 0;
	/** Indicates wether the object is busy.*/
	bool m_busy = false;
	/** Last read character.*/
	uint8_t m_availableChar;
	/** Slot for \c onDataAvailable .*/
	SEMF_SLOT(m_onDataAvailableSlot, Printer, *this, onDataAvailable);
	/** Slot for \c onCharWritten .*/
	SEMF_SLOT(m_onCharWrittenSlot, Printer, *this, onCharWritten);
	/** Slot for \c onStringWritten .*/
	SEMF_SLOT(m_onStringWrittenSlot, Printer, *this, onStringWritten);
	/** Slot for transmitting the error signal of \c m_uart .*/
	SEMF_SLOT(m_uartErrorSlot, Signal<Error>, error, emitSignal, Error);
	/** Class id for error handling.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Printer;
};
}  // namespace semf::esh
#endif  // SEMF_COMMUNICATION_ESH_PRINTER_H_
