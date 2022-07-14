/**
 * @file communication.h
 *
 * @date 03.05.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 * 			  For detailed information, read the license file in the project
 * 			  root directory.
 */

#ifndef INTERFACES_COMMUNICATION_H_
#define INTERFACES_COMMUNICATION_H_

#include <Core/Signals/signal.h>
#include <Core/error.h>
#include <cstdint>

namespace semf
{
/**
 * @brief This interface standardized the read and write interface
 *        for communication hardware like can, spi and all protocols.
 */
class Communication
{
public:
	virtual ~Communication() = default;

	/**
	 * @brief For writing data, \c dataWritten signal will be emitted after successful write.
	 * @param data Array, containing data to write.
	 * @param dataSize Size of write data.
	 */
	virtual void write(const uint8_t data[], size_t dataSize) = 0;

	/**
	 * @brief For reading data, \c dataAvailable signal will be emitted after successful read.
	 * @param buffer Buffer to store received bytes in.
	 * @param bufferSize Size of receive buffer.
	 */
	virtual void read(uint8_t buffer[], size_t bufferSize) = 0;

	/**
	 * @brief Stops the read process as fast as possible without destroying anything. The \c readStopped Signal gets emitted if the call was successful.
	 */
	virtual void stopRead() = 0;

	/**
	 * @brief Stops the write process as fast as possible without destroying anything. The \c writeStopped Signal gets emitted if the call was successful.
	 */
	virtual void stopWrite() = 0;

	/**
	 * @brief Communication hardware is busy reading at the moment.
	 * @return State of the communication hardware.
	 */
	virtual bool isBusyReading() const = 0;

	/**
	 * @brief Communication hardware is busy writing at the moment.
	 * @return State of the communication hardware.
	 */
	virtual bool isBusyWriting() const = 0;

	/** Signal is emitted after data from called \c write() function are written.*/
	Signal<> dataWritten;
	/** Signal is emitted after size of data from called \c read() function are received.*/
	Signal<> dataAvailable;
	/** Signal is emitted after writing was canceled by \c stopWrite().*/
	Signal<> writeStopped;
	/** Signal is emitted after writing was canceled by \c stopRead().*/
	Signal<> readStopped;
	/** Signal is emitted if any error happens in the communication hardware or hardware abstraction layer.*/
	Signal<1, Error> error;
};

}  /* namespace semf*/

#endif /* INTERFACES_COMMUNICATION_H_ */
