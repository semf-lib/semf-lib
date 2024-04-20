/**
 * @file communicationhardwaresynchronousmaster.h
 * @date 20.11.2018
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 * 			  For detailed information, read the license file in the project
 * 			  root directory.
 */

#ifndef SEMF_COMMUNICATION_COMMUNICATIONHARDWARESYNCHRONOUSMASTER_H_
#define SEMF_COMMUNICATION_COMMUNICATIONHARDWARESYNCHRONOUSMASTER_H_

#include <semf/communication/communicationhardware.h>

namespace semf
{
/**
 * @brief This interface standardized the read and write interface for synchronous master hardware,
 * like SPI master and I2C master.
 */
class CommunicationHardwareSynchronousMaster : public CommunicationHardware
{
public:
	virtual ~CommunicationHardwareSynchronousMaster() = default;

	Type type() const override
	{
		return CommunicationHardware::Type::SynchronousMaster;
	}
};
} /* namespace semf*/
#endif /* SEMF_COMMUNICATION_COMMUNICATIONHARDWARESYNCHRONOUSMASTER_H_ */
