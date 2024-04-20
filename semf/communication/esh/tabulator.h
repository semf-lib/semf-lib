/**
 * @file tabulator.h
 * @author jh
 * @date 11.03.2024
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved.
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_COMMUNICATION_ESH_TABULATOR_H_
#define SEMF_COMMUNICATION_ESH_TABULATOR_H_

#include <semf/communication/esh/command.h>
#include <semf/communication/esh/printer.h>
#include <semf/utils/core/lists/linkedlist.h>
#include <semf/utils/core/signals/signal.h>
#include <semf/utils/core/signals/slot.h>
#include <array>
#include <cstdlib>
#include <string_view>

namespace semf::esh
{
/**
 * @brief Handles the command auto completion.
 */
class Tabulator
{
public:
	/**
	 * @brief Error codes for this class. Error ID identify a unique error() / onError call (excluding transferring).
	 */
	enum class ErrorCode : uint8_t
	{
		Start_IsBusy = 0,
	};
	/**
	 * @brief Constructor.
	 * @param printer Printer for printing.
	 * @param commands List containing all commands.
	 * @param lineBuffer buffer containing the command line.
	 * @param lineBufferSize Size of \c lineBuffer .
	 * @param prompt Prompt string.
	 */
	Tabulator(Printer& printer, const LinkedList<Command>& commands, char lineBuffer[], size_t lineBufferSize, std::string_view prompt);
	Tabulator(const Tabulator& other) = delete;
	virtual ~Tabulator() = default;
	/**
	 * @brief Starts the auto completion.
	 * @param charCount Current length of the command line.
	 * @throws Start_IsBusy If object is busy.
	 */
	void start(size_t charCount);
	/**
	 * @brief Returns the busy-flag.
	 * @return \c true of object is busy.
	 */
	bool isBusy() const;
	/** Gets emitted when the auto complete process is done.*/
	SEMF_SIGNAL(done, size_t);
	/** Gets emitted when an unexpected error occures.*/
	SEMF_SIGNAL(error, Error);

private:
	/**
	 * @brief Checks if \c sub is a prefix of \c str .
	 * @param sub Prefix.
	 * @param str Full string
	 * @return \c true if \c sub is a prefix of \c str .
	 */
	bool prefixMatch(std::string_view sub, std::string_view str) const;
	/** Looks for matches and prints them.*/
	void searchMatches();
	/** Prints a single command name.*/
	void printCommandName();
	/** Continues the search in case it was suspended by an async function.*/
	void continueSearch();
	/** Updates the prompt with a result.*/
	void updatePrompt();
	/** Prints the found command.*/
	void printCommand();
	/** Clears the busy flag and emits \c done .*/
	void onDone();
	/** Printer for printing.*/
	Printer& m_printer;
	/** All commands.*/
	const LinkedList<Command>& m_commands;
	/** Current command line.*/
	char* const m_lineBuffer;
	/** Size of \c m_lineBuffer. */
	const size_t m_lineBufferSize;
	/** Prompt string.*/
	const std::string_view kPrompt;
	/** Current search position.*/
	LinkedList<Command>::ConstIterator m_current;
	/** Last match.*/
	LinkedList<Command>::ConstIterator m_lastMatch;
	/**Busy flag.*/
	bool m_busy = false;
	/** New command length.*/
	size_t m_charCount = 0;
	/** Number of found matches.*/
	uint32_t m_matchCount = 0;
	/** Slot for \c printCommandName .*/
	SEMF_SLOT(m_printCommandNameSlot, Tabulator, *this, printCommandName);
	/** Slot for \c continueSearch .*/
	SEMF_SLOT(m_continueSearchSlot, Tabulator, *this, continueSearch);
	/** Slot for \c updatePrompt .*/
	SEMF_SLOT(m_updatePromptSlot, Tabulator, *this, updatePrompt);
	/** Slot for \c printCommand .*/
	SEMF_SLOT(m_printCommandSlot, Tabulator, *this, printCommand);
	/** Slot for \c onDone .*/
	SEMF_SLOT(m_onDoneSlot, Tabulator, *this, onDone);
	/** Class Id for error handling.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Tabulator;
};
}  // namespace semf::esh
#endif  // SEMF_COMMUNICATION_ESH_TABULATOR_H_
