/**
 * @file communicationhardware.h
 * @date 20.11.2018
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 * 			  For detailed information, read the license file in the project
 * 			  root directory.
 */

#ifndef SEMF_COMMUNICATION_COMMUNICATIONHARDWARE_H_
#define SEMF_COMMUNICATION_COMMUNICATIONHARDWARE_H_

#include <semf/app/communication/communication.h>
#include <semf/utils/core/signals/signal.h>

namespace semf
{
/**
 * @brief This interface standardized the read and write interface between the communication hardware
 *        like CAN, SPI and all protocols.
 */
class CommunicationHardware : public app::Communication
{
public:
	/** To identify the hardware type in higher classes like protocols.*/
	enum class Type
	{
		SynchronousMaster = 1,  //!< Synchronous master like \c I2cMasterHardware or \c SpiMasterHardware.
		SynchronousSlave,       //!< Synchronous slave like \c I2cSlaveHardware or \c SpiSlaveHardware.
		Asynchronous,           //!< Asynchronous interface like \c UartHardware.
	};
	/** Frame Modes (used e.g. in SPI and I2C)*/
	enum class Frame : uint8_t
	{
		First = 0x01,        //!< start condition, but no stop condition
		Next = 0x08,         //!< no start AND no stop condition
		Last = 0x10,         //!< stop condition
		FirstAndLast = 0x11  //!< start AND stop condition
	};

	CommunicationHardware() = default;
	explicit CommunicationHardware(const CommunicationHardware& other) = delete;
	virtual ~CommunicationHardware() = default;

	/**Initializes the hardware.*/
	virtual void init() = 0;
	/**Deinitializes the hardware*/
	virtual void deinit() = 0;
	/**
	 * @brief Returns the hardware type.
	 * @return Hardware type.
	 */
	virtual Type type() const = 0;
	/**
	 * @brief Sets the selected usage of start and stop condition
	 * @param frame frame mode
	 */
	virtual void setFrame(Frame frame) = 0;
};
} /* namespace semf*/
#endif /* SEMF_COMMUNICATION_COMMUNICATIONHARDWARE_H_ */
