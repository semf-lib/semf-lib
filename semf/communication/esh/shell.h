/**
 * @file shell.h
 * @author jh
 * @date 11.03.2024
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved.
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_COMMUNICATION_ESH_SHELL_H_
#define SEMF_COMMUNICATION_ESH_SHELL_H_

#include <semf/utils/core/lists/linkedlist.h>
#include <semf/utils/core/signals/signal.h>
#include <semf/communication/esh/arrowcontrol.h>
#include <semf/communication/esh/command.h>
#include <semf/communication/esh/history.h>
#include <semf/communication/esh/printer.h>
#include <semf/communication/esh/processor.h>
#include <semf/communication/esh/tabulator.h>
#include <semf/communication/uarthardware.h>
#include <array>
#include <cstdint>
#include <string_view>

namespace semf::esh
{
/**
 * @brief This class manages a semf's embedded shell (esh) operating on a single UART. The esh can be used for executing custom command and running them in a
 * main-function-like environment. For using the esh create an instance of this class. Call the \c start function for staring the shell and call the \c loop
 * function inside an endless loop to enable command execution. For adding command to the shell simply create at least one \c esh::Command object.
 */
class Shell
{
public:
	/**
	 * @brief Configuarion parameters of the shell.
	 */
	struct Config
	{
		/** Buffer for the command line. Its size indicates the max length of a command.*/
		char* const lineBuffer;
		/** Size of \c lineBuffer .*/
		const size_t lineBufferSize;
		/** argv buffer. Upper bound is \c lineBufferSize / 2. Lower bound is 1.*/
		char** const argvBuffer;
		/** Length of \c argvBuffer .*/
		const size_t argvBufferSize;
		/** Buffer for storing the history. Its size should be numberOfEntries * \c lineBufferSize .*/
		char* historyBuffer;
		/** Number of history entries.*/
		const size_t numberOfEntries;
		/** Enables shell echoing.*/
		bool echo;
		/** Prompt string, i.e. <code>"> "</code>.*/
		std::string_view prompt;
	};
	/**
	 * @brief Constructor.
	 * @param uart UART to use.
	 * @param config esh's configuration.
	 */
	Shell(UartHardware& uart, const Config& config);
	Shell(const Shell& other) = delete;
	virtual ~Shell() = default;
	/**
	 * @brief Adds a command to the shell.
	 * @param cmd Command to add to the shell.
	 * @note Since this is done by \c esh::Command the user shouldn't have to call this function.
	 */
	void addCommand(Command& cmd);
	/**
	 * @brief Starts the reading process of the shell. The user should call this function once.
	 */
	void start();
	/**
	 * @brief Performs the command execution.
	 */
	void loop();
	/** Gets emitted on errors.*/
	SEMF_SIGNAL(error, Error);

private:
	/** ASCII DEL character.*/
	static constexpr char kDelete = '\177';
	/** ASCII ESC character.*/
	static constexpr char kEscape = '\33';

	/**
	 * @brief Handles incomming characters accordingly.
	 * @param character Received character.
	 */
	void onCharAvailable(char character);
	/**
	 * @brief Called when the autocompletion is done.
	 * @param charCount updated value for \c m_count .
	 */
	void onTabDone(size_t charCount);
	/** Restarts the shell after the command execution is done. */
	void onExecuted();
	/** Handles the history backwards.*/
	void onArrowUp();
	/** Handles the history forwards.*/
	void onArrowDown();
	/** Handles the overlap of \c onArrowUp and \c onArrowDown .*/
	void updateStateAndPrint();
	/** Passes unexpected errors onto the user.*/
	void onError(Error error);
	/** Starts reading a new character.*/
	void readNextCharacter();

	/** esh's configuration.*/
	const Config m_config;
	/** esh's history.*/
	History m_history;
	/** Printer for string based communication.*/
	Printer m_printer;
	/** List of all commands.*/
	LinkedList<Command> m_commands;
	/** Tab auto complete.*/
	Tabulator m_tab;
	/** Command executer.*/
	Processor m_processor;
	/** Arrow handler.*/
	ArrowControl m_arrow;
	/** Current characters on command line.*/
	int m_count = 0;
	/** Slot for \c onCharAvailable .*/
	SEMF_SLOT(m_onCharAvailableSlot, Shell, *this, onCharAvailable, char);
	/** Slot for \c start .*/
	SEMF_SLOT(m_restartShell, Shell, *this, start);
	/** Slot for \c onTabDone .*/
	SEMF_SLOT(m_onTabDoneSlot, Shell, *this, onTabDone, size_t);
	/** Slot for \c onExecuted .*/
	SEMF_SLOT(m_onExecutedSlot, Shell, *this, onExecuted);
	/** Slot for \c onArrowUp .*/
	SEMF_SLOT(m_onArrowUpSlot, Shell, *this, onArrowUp);
	/** Slot for \c onArrowDown .*/
	SEMF_SLOT(m_onArrowDownSlot, Shell, *this, onArrowDown);
	/** Slot for \c readCharacter .*/
	SEMF_SLOT(m_readNextCharSlot, Shell, *this, readNextCharacter);
	/** Slot for \c onError .*/
	SEMF_SLOT(m_onErrorSlot, Shell, *this, onError, Error);
};
}  // namespace semf::esh
#endif  // SEMF_COMMUNICATION_ESH_SHELL_H_
