/**
 * @file communicationhardwareasynchronous.h
 * @date 20.11.2018
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 * 			  For detailed information, read the license file in the project
 * 			  root directory.
 */

#ifndef SEMF_COMMUNICATION_COMMUNICATIONHARDWAREASYNCHRONOUS_H_
#define SEMF_COMMUNICATION_COMMUNICATIONHARDWAREASYNCHRONOUS_H_

#include <semf/communication/communicationhardware.h>

namespace semf
{
/**
 * @brief This interface standardized the read and write interface for asynchronous hardware,
 * like UART or CAN.
 */
class CommunicationHardwareAsynchronous : public CommunicationHardware
{
public:
	CommunicationHardwareAsynchronous() = default;
	explicit CommunicationHardwareAsynchronous(const CommunicationHardwareAsynchronous& other) = delete;
	virtual ~CommunicationHardwareAsynchronous() = default;

	Type type() const override
	{
		return CommunicationHardware::Type::Asynchronous;
	}

	/**
	 * @brief Sets the selected usage of start and stop condition
	 * @attention Does not impact asynchronous bus systems and will be ignored.
	 * @param frame frame mode
	 */
	void setFrame(Frame frame) final
	{
		(void)frame;
	}
};
} /* namespace semf*/
#endif /* SEMF_COMMUNICATION_COMMUNICATIONHARDWAREASYNCHRONOUS_H_ */
