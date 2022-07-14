/**
 * @file canhardware.h
 *
 * @date 24.10.2018
 * @author aa
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 * 			  For detailed information, read the license file in the project
 * 			  root directory.
 */

#ifndef INTERFACES_CANHARDWARE_H_
#define INTERFACES_CANHARDWARE_H_

#include <Interfaces/communicationhardwareasynchronous.h>
#include <Interfaces/can.h>

namespace semf
{
/**
 * @brief Interface for using CAN.
 * @note For \c Can \c read() function will only register the receive buffer
 * and allow handling the received data.
 */
class CanHardware : public Can, public CommunicationHardwareAsynchronous
{
public:
	/**
	 * @brief Error codes for this class. Error ID identify a unique error() / onError call (excluding transferring).
	 */
	enum class ErrorCode : uint8_t
	{
		Write_IsBusy = 0,
		Write_DataIsNullptr,
		Write_DataSizeIsZero,
		Read_DataIsNullptr,
		Read_DataSizeIsZero,
		Request_IsBusy
	};
	virtual ~CanHardware() = default;
	/**
	 * @copydoc CommunicationHardwareAsynchronous::write()
	 * @note Message id has to be set first by calling \c setMessageId().
	 * @throws Write_IsBusy If this is busy.
	 * @throws Write_DataIsNullptr If data is nullptr.
	 * @throws Write_DataSizeIsZero If dataSize is zero.
	 */
	void write(const uint8_t data[], size_t dataSize);
	/**
	 * @brief For setting the read buffer. This has be done for getting a
	 * \c dataAvailable signal for handling the data.
	 * @param buffer Buffer to store received bytes in.
	 * @param bufferSize Size of receive buffer.
	 * @throws Read_DataIsNullptr If data is nullptr.
	 * @throws Read_DataSizeIsZero If dataSize is zero.
	 */
	void read(uint8_t buffer[], size_t bufferSize);
	/**
	 * @copydoc Communication::isBusyReading()
	 * @attention Returns always false, is not used here.
	 */
	bool isBusyReading() const;
	bool isBusyWriting() const;
	/**
	 * @copydoc Can::request()
	 * @throws Request_IsBusy If this is busy.
	 */
	void request();

protected:
	/**
	 * @brief Sets the busy flag for writing.
	 * @note in the functions \c write(), \c read()
	 * busy is set to \c true. In \c onDataAvailable(), \c onDataWritten()
	 * and \c onError() is busy reseted to \c false.
	 * @param isBusy \c true for busy, \c false for idle.
	 */
	void setBusyWriting(bool isBusy);

	/**
	 * @brief Sets the read buffer for having the possibility to handle
	 * the received data.
	 * @param buffer Buffer to store received bytes in.
	 * @param bufferSize Size of receive buffer.
	 */
	virtual void setReadBuffer(uint8_t buffer[], size_t bufferSize) = 0;

	/**
	 * @brief Hardware will write data
	 * @param data data array
	 * @param dataSize data array size
	 */
	virtual void writeHardware(const uint8_t data[], size_t dataSize) = 0;

	/**
	 * @brief Hardware will request data on specific id. Id can be set by calling \c setMessageId().
	 */
	virtual void requestHardware() = 0;

	/**
	 * @brief Is called after data are written by the hardware.
	 * Will throw \c dataWritten signal.
	 */
	void onDataWritten();

	/**
	 * @brief Is called after data is available in the hardware.
	 * Will throw \c dataAvailable signal.
	 */
	void onDataAvailable();

	/**
	 * @brief Is called if an error occurred by hardware read or write access.
	 * Will throw \c error signal.
	 * @param thrown A thrown error object.
	 */
	void onError(Error thrown);

	/**Slot for hardware has finished request.*/
	void onDataRequested();

private:
	/**Flag for hardware is busy writing.*/
	bool m_isBusyWriting = false;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::CanHardware;
};

}  /* namespace semf */

#endif  /* INTERFACES_CANHARDWARE_H_ */
