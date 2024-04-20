/**
 * @file command.h
 * @author jh
 * @date 11.03.2024
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved.
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_COMMUNICATION_ESH_COMMAND_H_
#define SEMF_COMMUNICATION_ESH_COMMAND_H_

#include <semf/communication/uarthardware.h>
#include <semf/communication/esh/printer.h>
#include <semf/utils/core/lists/linkedlist.h>
#include <semf/utils/core/signals/slotbase.h>
#include <semf/utils/core/signals/signal.h>
#include <string_view>

namespace semf::esh
{
class Shell;

/**
 * @brief Wrapper class for shell command. The user has to create object of this class for adding their functionality to the shell.
 */
class Command : public LinkedList<Command>::Node
{
public:
	/**
	 * @brief Constructor. Adds the command to its shell.
	 * @param name Name of the command. Sending the name via to the shell triggers this command.
	 * @param help Help description.
	 * @param callback Callback slot.
	 * @param esh The shell this command belongs to.
	 */
	Command(std::string_view name, std::string_view help, SlotBase<int, char**, Printer&, int&>& callback, Shell& esh);
	/**
	 * @brief Constructor. Adds the command to its shell, without providing a callback immediately.
	 * @param name Name of the command. Sending the name via to the shell triggers this command.
	 * @param help Help description.
	 * @param esh The shell this command belongs to.
	 */
	Command(std::string_view name, std::string_view help, Shell& esh);
	/**
	 * @brief Constructor. Adds the command to its shell, without providing a callback immediately.
	 * @param name Name of the command. Sending the name via to the shell triggers this command.
	 * @param help Help description.
	 */
	Command(std::string_view name, std::string_view help);
	Command(const Command& other) = delete;
	virtual ~Command() = default;
	/**
	 * @brief Returns the command's name.
	 * @return Command's name.
	 */
	std::string_view name() const;
	/**
	 * @brief Returns the command's help text.
	 * @return Command's help text.
	 */
	std::string_view help() const;
	/** Gets emitted when the command is about to be executed.*/
	SEMF_SIGNAL(command, int, char**, Printer&, int&);

private:
	/** Name of the command.*/
	const std::string_view m_name;
	/** Help description.*/
	const std::string_view m_help;
};
}  // namespace semf::esh
#endif  // SEMF_COMMUNICATION_ESH_COMMAND_H_
