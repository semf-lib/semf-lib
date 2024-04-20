/**
 * @file history.h
 * @author jh
 * @date 12.03.2024
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved.
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_COMMUNICATION_ESH_HISTORY_H_
#define SEMF_COMMUNICATION_ESH_HISTORY_H_

#include <semf/communication/uarthardware.h>
#include <array>
#include <cstdint>
#include <string_view>

namespace semf::esh
{
/**
 * @brief Manages the shell's history.
 */
class History
{
public:
	/**
	 * @brief Constructor.
	 * @param historyBuffer Buffer for storing the history. The size of this buffer should be \c historySize * \c lineSize .
	 * @param numberOfEntries Number of history entries.
	 * @param lineSize Size of a single history entry.
	 */
	History(char* historyBuffer, size_t numberOfEntries, size_t lineSize);
	History(const History& other) = delete;
	virtual ~History() = default;
	/**
	 * @brief Inserts a command.
	 * @param command Command to insert.
	 */
	void insert(std::string_view command);
	/**
	 * @brief Performes a lookup based on an up-arrow key stroke.
	 * @param commandBuffer [out] Buffer to store the result.
	 * @note It is assumed that the size of \c commandBuffer is \c m_lineSize .
	 */
	void handleArrowUp(char commandBuffer[]);
	/**
	 * @brief Performes a lookup based on an down-arrow key stroke.
	 * @param commandBuffer [out] Buffer to store the result.
	 * @note It is assumed that the size of \c commandBuffer is \c m_lineSize .
	 */
	void handleArrowDown(char commandBuffer[]);

private:
	/** History buffer.*/
	char* const m_history;
	/** Number of history entries.*/
	const size_t m_numberOfEntries;
	/** Size of a single history entry.*/
	const size_t m_lineSize;
	/** Number of saved commands*/
	int32_t m_savedCommands = 0;
	/** Current history index.*/
	int32_t m_index = 0;
};
}  // namespace semf::esh
#endif  // SEMF_COMMUNICATION_ESH_HISTORY_H_
