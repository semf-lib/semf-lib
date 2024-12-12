/**
 * @file uart.h
 * @date 25.02.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_COMMUNICATION_UARTHARDWARE_H_
#define SEMF_COMMUNICATION_UARTHARDWARE_H_

#include <semf/communication/communicationhardwareasynchronous.h>
#include <semf/communication/uart.h>

namespace semf
{
/**
 * @brief Class for using UART hardware
 */
class UartHardware : public Uart, public CommunicationHardwareAsynchronous
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
		Read_IsBusy,
		Read_BufferIsNullptr,
		Read_BufferSizeIsZero
	};

	UartHardware() = default;
	explicit UartHardware(const UartHardware& other) = delete;
	virtual ~UartHardware() = default;

	/**
	 * @copydoc CommunicationHardwareAsynchronous::write()
	 * @throws Write_IsBusy If this is busy.
	 * @throws Write_DataIsNullptr If data is a nullptr.
	 * @throws Write_DataSizeIsZero If dataSize is zero.
	 */
	void write(const uint8_t data[], size_t dataSize) override;
	/**
	 * @copydoc CommunicationHardwareAsynchronous::read()
	 * @throws Read_IsBusy If this is busy.
	 * @throws Read_BufferIsNullptr If buffer is a nullptr.
	 * @throws Read_BufferSizeIsZero If bufferSize is zero.
	 */
	void read(uint8_t buffer[], size_t bufferSize) override;
	bool isBusyReading() const override;
	bool isBusyWriting() const override;

protected:
	/**
	 * @brief Sets the busy flag for reading.
	 * @note in the functions \c write(), \c read()
	 * busy is set to \c true. In \c onDataAvailable(), \c onDataWritten()
	 * and \c onError() is busy reseted to \c false.
	 * @param isBusy \c true for busy, \c false for idle.
	 */
	void setBusyReading(bool isBusy);
	/**
	 * @brief Sets the busy flag for writing.
	 * @note in the functions \c write(), \c read()
	 * busy is set to \c true. In \c onDataAvailable(), \c onDataWritten()
	 * and \c onError() is busy reseted to \c false.
	 * @param isBusy \c true for busy, \c false for idle.
	 */
	void setBusyWriting(bool isBusy);
	/**
	 * @brief Hardware will write data in interrupt mode.
	 * @param data data array.
	 * @param dataSize data array size.
	 */
	virtual void writeHardware(const uint8_t data[], size_t dataSize) = 0;
	/**
	 * @brief Hardware will read data in interrupt mode.
	 * @param buffer data array.
	 * @param bufferSize data array size.
	 */
	virtual void readHardware(uint8_t buffer[], size_t bufferSize) = 0;
	/**Is called after data are written by the hardware. Will emit \c dataWritten signal.*/
	void onDataWritten();
	/**Is called after data is available in the hardware. Will emit \c dataAvailable signal.*/
	void onDataAvailable();
	/**
	 * @brief Is called if an error occurred by hardware read or write access.
	 * Will throw \c error signal.
	 * @param thrown A thrown error object.
	 */
	void onError(Error thrown);

private:
	/**Flag for hardware is busy reading.*/
	bool m_isBusyReading = false;
	/**Flag for hardware is busy writing.*/
	bool m_isBusyWriting = false;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::UartHardware;
};
} /* namespace semf */
#endif /* SEMF_COMMUNICATION_UARTHARDWARE_H_ */
