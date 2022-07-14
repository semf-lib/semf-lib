/**
 * @file communicationhardwaresynchronousmaster.h
 *
 * @date 20.11.2018
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 * 			  For detailed information, read the license file in the project
 * 			  root directory.
 */

#ifndef INTERFACES_COMMUNICATIONHARDWARESYNCHRONOUSMASTER_H_
#define INTERFACES_COMMUNICATIONHARDWARESYNCHRONOUSMASTER_H_

#include <Interfaces/communicationhardware.h>

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

	Type type() const
	{
		return CommunicationHardware::Type::SynchronousMaster;
	}
};

}  /* namespace semf*/

#endif /* INTERFACES_COMMUNICATIONHARDWARESYNCHRONOUSMASTER_H_ */
