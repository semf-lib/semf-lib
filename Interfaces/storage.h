/**
 * @file storage.h
 *
 * @date 18.04.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in 
 *            the project root directory.
 */

#ifndef INTERFACES_STORAGE_H_
#define INTERFACES_STORAGE_H_

#include <Core/Signals/signal.h>
#include <Core/error.h>
#include <cstdint>

namespace semf
{
/**
 * @brief Interface for using storage memories/devices (e.g. flash or eeprom).
 */
class Storage
{
public:
	virtual ~Storage() = default;

	/**
	 * @brief Writes data into the storage.
	 * <ul><li>\c dataWritten signal will be emitted for all data are written.</li>
	 * <li>\c error signal will be emitted if an error occurred.</li></ul>
	 * @param address The address where the data will be written to.
	 * @param data Pointer to the data to be written.
	 * @param dataSize Size of data to be written in bytes.
	 */
	virtual void write(uint32_t address, const uint8_t data[], size_t dataSize) = 0;

	/**
	 * @brief Reads data from the storage into a given read buffer.
	 * <ul><li>\c dataAvailable signal will be emitted for all data are read.</li>
	 * <li>\c error signal will be emitted if an error occurred.</li></ul>
	 * @param address The address from which the data will be read.
	 * @param buffer Pointer at which the read data is stored.
	 * @param bufferSize Size of the data to be read in bytes.
	 */
	virtual void read(uint32_t address, uint8_t buffer[], size_t bufferSize) = 0;

	/**
	 * @brief Returns if the storage is busy reading, writing or e.g. erasing.
	 * @return \c true for busy, otherwise \c false.
	 */
	virtual bool isBusy() const = 0;

	Signal<> dataAvailable;  /**< The signal is triggered after the data has been read completely.*/
	Signal<> dataWritten; 	 /**< The signal is triggered after the data has been written completely.*/
	Signal<1, Error> error;			 /**< The signal is triggered after an error occurred.*/
};

} /* namespace semf */

#endif /* INTERFACES_STORAGE_H_ */
