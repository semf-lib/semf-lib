/**
 * @file processor.h
 * @author jh
 * @date 11.03.2024
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved.
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_COMMUNICATION_ESH_PROCESSOR_H_
#define SEMF_COMMUNICATION_ESH_PROCESSOR_H_

#include <semf/communication/esh/command.h>
#include <semf/communication/esh/printer.h>
#include <semf/utils/core/lists/linkedlist.h>
#include <semf/utils/core/signals/signal.h>
#include <semf/utils/core/signals/slot.h>
#include <array>

namespace semf::esh
{
/**
 * @brief Manages esh's command parsing and execution.
 */
class Processor
{
public:
	/**
	 * @brief Error codes for this class. Error ID identify a unique error() / onError call (excluding transferring).
	 */
	enum class ErrorCode : uint8_t
	{
		LoadCommand_IsBusy = 0,
	};
	/**
	 * @brief Constructor.
	 * @param commands List of available commands.
	 * @param printer Printer for printing.
	 * @param lineBuffer Buffer containing the current command line.
	 * @param lineBufferSize Size of \c lineBuffer .
	 * @param argvBuffer Pointer array of char* for pointing to arguments. The upper bound for this buffer is \c lineBufferSize / 2. The lower bound is 1 (no
	 * command takes parameters).
	 * @param argvBufferSize Length of \c argvBuffer .
	 */
	Processor(const LinkedList<Command>& commands, Printer& printer, char lineBuffer[], size_t lineBufferSize, char** argvBuffer, size_t argvBufferSize);
	Processor(const Processor& other) = delete;
	virtual ~Processor() = default;
	/**
	 * @brief Load a command for execution according to the current command line.
	 * @throws LoadCommand_IsBusy If object is busy.
	 */
	void loadCommand();
	/**
	 * @brief Execute the currenty loaded command. Returns with no error if no command is loaded. Unloads the loaded command after execution.
	 */
	void execLoadedCommand();
	/** Gets emitted after the loaded command was executed.*/
	SEMF_SIGNAL(done);
	/** Gets emitted on errors.*/
	SEMF_SIGNAL(error, Error);

private:
	/** Initializes \c m_argc and \c m_argv .*/
	void parseLine();
	/** Start the error message printing.*/
	void printError();
	/** Prints the entered command name.*/
	void onQuotationMarkPrinted();
	/** Prints "command not found" and emits \c done .*/
	void onNamePrinted();
	/** Prints clears the busy flag and emits \c done .*/
	void onDone();
	/** List containing all commands.*/
	const LinkedList<Command>& m_commands;
	/** Gets passed to commands for execution.*/
	Printer& m_printer;
	/** Current command line.*/
	char* const m_lineBuffer;
	/** Size of \c m_lineBuffer. */
	const size_t m_lineBufferSize;
	/** Argument vector.*/
	char** const m_argv;
	/** Length of \c m_argv. */
	const size_t m_argvSize;
	/** Inidcates wether a command is currently executed.*/
	bool m_busy = false;
	/** Loaded command.*/
	const Command* m_currentCommand = nullptr;
	/** Argument count.*/
	int m_argc;
	/** Last exit code.*/
	int m_lastExitStatus = 0;
	/** Slot for \c onQuotationMarkPrinted .*/
	SEMF_SLOT(m_onQuotationMarkPrintedSlot, Processor, *this, onQuotationMarkPrinted);
	/** Slot for \c onNamePrinted .*/
	SEMF_SLOT(m_onNamePrintedSlot, Processor, *this, onNamePrinted);
	/** Slot for \c onDone .*/
	SEMF_SLOT(m_onDoneSlot, Processor, *this, onDone);
	/** Class Id for error handling.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Processor;
};
}  // namespace semf::esh
#endif  // SEMF_COMMUNICATION_ESH_PROCESSOR_H_
