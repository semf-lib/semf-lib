/**
 * @file can.h
 * @date 24.10.2018
 * @author aa
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 * 			  For detailed information, read the license file in the project
 * 			  root directory.
 */

#ifndef SEMF_COMMUNICATION_CAN_H_
#define SEMF_COMMUNICATION_CAN_H_

#include <semf/utils/core/signals/signal.h>
#include <cstdint>

namespace semf
{
/**
 * @brief Interface for CAN specific functionalities, which are not solved in a generic way in \c CommunicationHardware.
 *
 * @see \c CommunicationHardware
 * @see \c CanHardware
 */
class Can
{
public:
	virtual ~Can() = default;

	/**Sends a CAN request. Message id has to be set first by calling \c setMessageId().*/
	virtual void request() = 0;
	/**
	 * @brief Returns the message ID for reading data. Use this function after reading the data via read-function
	 * @return message ID of read data
	 */
	virtual uint32_t messageId() const = 0;
	/**
	 * @brief Sets the message ID for writing data. Use this function before writing the data via write-function
	 * @param id message ID of read data
	 */
	virtual void setMessageId(uint32_t id) = 0;
	/**
	 * @brief Sets the speed
	 * @param hz frequency in hz
	 */
	virtual void setFrequency(uint32_t hz) = 0;
	/**
	 * @brief Sets a message hardware receive filter.
	 * @param filterBank Index of filter bank.
	 * @param messageId Message id for allowing to receive.
	 * @param messageIdMask Message id range for allowing to receive.
	 */
	virtual void setFilter(uint32_t filterBank, uint32_t messageId, uint32_t messageIdMask) = 0;

	/** Signal is fired after data requested is received*/
	Signal<> dataRequested;
};
} /* namespace semf */
#endif /* SEMF_COMMUNICATION_CAN_H_ */
