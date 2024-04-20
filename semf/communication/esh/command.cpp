/**
 * @file command.cpp
 * @author jh
 * @date 11.03.2024
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved.
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/communication/esh/command.h>
#include <semf/communication/esh/shell.h>

namespace semf::esh
{
Command::Command(std::string_view name, std::string_view help, SlotBase<int, char**, Printer&, int&>& callback, Shell& esh)
: Command(name, help, esh)
{
	command.connect(callback);
}

Command::Command(std::string_view name, std::string_view help, Shell& esh)
: Command(name, help)
{
	esh.addCommand(*this);
}

Command::Command(std::string_view name, std::string_view help)
: m_name(name),
  m_help(help)
{}

std::string_view Command::name() const
{
	return m_name;
}

std::string_view Command::help() const
{
	return m_help;
}
}  // namespace semf::esh
