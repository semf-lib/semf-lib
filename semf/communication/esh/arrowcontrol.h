/**
 * @file arrowcontrol.h
 * @author jh
 * @date 11.03.2024
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved.
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_COMMUNICATION_ESH_ARROWCONTROL_H_
#define SEMF_COMMUNICATION_ESH_ARROWCONTROL_H_

#include <semf/communication/esh/printer.h>
#include <semf/utils/core/signals/signal.h>
#include <semf/utils/core/signals/slot.h>

namespace semf::esh
{
/**
 * @brief Detect up-arrow and down-arrow key strokes via the \c Printer.
 */
class ArrowControl
{
public:
	/**
	 * @brief Error codes for this class. Error ID identify a unique error() / onError call (excluding transferring).
	 */
	enum class ErrorCode : uint8_t
	{
		Start_IsBusy = 0,
		OnSecondChar_UnexpectedChar,
		OnThirdChar_UnexpectedChar,
	};
	/**
	 * @brief Constructor.
	 * @param printer Printer to read from
	 * @param count Reference on a variable holding the currently available char. Necessarry for cleanup.
	 * @param echo Indicated wether the shell echos its input.
	 */
	ArrowControl(Printer& printer, const int& count, bool echo);
	ArrowControl(const ArrowControl& other) = delete;
	virtual ~ArrowControl() = default;
	/**
	 * @brief Starts the reading process.
	 * @note This implementation expects that this function is called after receiving an ESC charcter ( \c '\33' ).
	 * @throws Start_IsBusy If object is busy
	 * @throws OnSecondChar_UnexpectedChar If second received character is not ’[’
	 * @throws OnThirdChar_UnexpectedChar If third received character is not 'A' nor 'B'.
	 */
	void start();
	/** Emitted after up-arrow was pressed.*/
	SEMF_SIGNAL(up);
	/** Emitted after down-error was pressed.*/
	SEMF_SIGNAL(down);
	/** Emited on error.*/
	SEMF_SIGNAL(error, Error);

private:
	/**
	 * @brief Checks whether second character is '['.
	 * @param c Received character.
	 */
	void onSecondChar(char c);
	/**
	 * @brief Checks whether third character is 'A' or 'B'.
	 * @param c Received character.
	 */
	void onThirdChar(char c);
	/** Emits the class's signals after the shell has been cleared.*/
	void onCleared();

	/** Printer for uart handling.*/
	Printer& m_printer;
	/** Reference on a variable holding the currently available char. Necessarry for cleanup.*/
	const int& m_count;
	/** Indicated wether the shell echos its input.*/
	const bool m_echo;
	/** Indicated wether the object is busy.*/
	bool m_busy;
	/** Last read character.*/
	char m_c;
	/** Slot for \c onSecondChar. */
	SEMF_SLOT(m_onSecondCharSlot, ArrowControl, *this, onSecondChar, char);
	/** Slot for \c onThirdChar. */
	SEMF_SLOT(m_onThirdCharSlot, ArrowControl, *this, onThirdChar, char);
	/** Slot for \c onCleared. */
	SEMF_SLOT(m_onCleared, ArrowControl, *this, onCleared);
	/** ClassID for error handling. */
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::ArrowControl;
};

}  // namespace semf::esh
#endif  // SEMF_COMMUNICATION_ESH_ARROWCONTROL_H_
