/**
 * @file communicationhardwaresynchronousslave.h
 * @date 20.11.2018
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 * 			  For detailed information, read the license file in the project
 * 			  root directory.
 */

#ifndef SEMF_COMMUNICATION_COMMUNICATIONHARDWARESYNCHRONOUSSLAVE_H_
#define SEMF_COMMUNICATION_COMMUNICATIONHARDWARESYNCHRONOUSSLAVE_H_

#include <semf/communication/communicationhardware.h>

namespace semf
{
/**
 * @brief This interface standardized the read and write interface for synchronous slave hardware,
 * like SPI slave and I2C slave.
 */
class CommunicationHardwareSynchronousSlave : public CommunicationHardware
{
public:
	virtual ~CommunicationHardwareSynchronousSlave() = default;

	Type type() const override
	{
		return CommunicationHardware::Type::SynchronousSlave;
	}
};
} /* namespace semf*/
#endif /* SEMF_COMMUNICATION_COMMUNICATIONHARDWARESYNCHRONOUSSLAVE_H_ */
