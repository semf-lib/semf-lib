/**
 * @file i2cmasterhardware.h
 * @date 20.04.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_COMMUNICATION_I2CMASTERHARDWARE_H_
#define SEMF_COMMUNICATION_I2CMASTERHARDWARE_H_

#include <semf/communication/i2c.h>
#include <semf/communication/communicationhardwaresynchronousmaster.h>

namespace semf
{
/**
 * @brief For using the I2C in master mode.
 */
class I2cMasterHardware : public I2c, public CommunicationHardwareSynchronousMaster
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
		Read_BufferSizeIsZero,
		SetAddress_IsBusy
	};

	virtual ~I2cMasterHardware() = default;

	/**
	 * @brief Checks if the given address is available on the bus. Emits \c address found if successful (ACK). \c error signal will be emitted, if no slave
	 * answers (NACK).
	 * @param address 7 bit I2C slave address.
	 */
	virtual void checkAddress(uint8_t address) = 0;
	/**
	 * @copydoc CommunicationHardwareSynchronousMaster::write()
	 * @throws Write_IsBusy If this is busy.
	 * @throws Write_DataIsNullptr If data is nullptr
	 * @throws Write_DataSizeIsZero If dataSize is zero.
	 */
	void write(const uint8_t data[], size_t dataSize) override;
	/**
	 * @copydoc CommunicationHardwareSynchronousMaster::read()
	 * @throws Read_IsBusy If this is busy.
	 * @throws Read_BufferIsNullptr If buffer is nullptr.
	 * @throws Read_BufferSizeIsZero If bufferSize is zero.
	 */
	void read(uint8_t buffer[], size_t bufferSize) override;
	bool isBusyReading() const override;
	bool isBusyWriting() const override;
	/**
	 *  @brief Returns the I2C slave device address.
	 *  @return Device address.
	 */
	uint8_t address() const override;
	/**
	 * @copydoc I2c::setAddress()
	 * @throws SetAddress_IsBusy If this is busy.
	 */
	void setAddress(uint8_t address) override;
	void setFrame(CommunicationHardware::Frame frame) override;

	/**Gets emitted after calling \c checkAddress successfully.*/
	semf::Signal<> addressFound;

protected:
	/**
	 * @brief Sets the busy flag.
	 * @note in the functions \c write(), \c read()
	 * busy is set to \c true. In \c onDataAvailable(), \c onDataWritten()
	 * and \c onError() is busy reseted to \c false.
	 * @param isBusy \c true for busy, \c false for idle.
	 */
	void setBusy(bool isBusy);
	/**
	 * @brief Returns the actual frame mode setting.
	 * @return Frame mode for setting start and stop condition.
	 */
	CommunicationHardware::Frame frame() const;
	/**
	 * @brief Hardware will write data.
	 * After finished write operation, \c onDataWritten() function will be called.
	 * @param data Array of data to write.
	 * @param dataSize Size of \c data.
	 */
	virtual void writeHardware(const uint8_t data[], size_t dataSize) = 0;
	/**
	 * @brief Hardware will read data.
	 * After finished read operation, \c onDataAvailable() function will be called.
	 * @param buffer Buffer to store received data in.
	 * @param bufferSize Size of \c data.
	 */
	virtual void readHardware(uint8_t buffer[], size_t bufferSize) = 0;
	/**
	 * @brief Configures the frequency of the bus.
	 * @param hz Frequency in hertz.
	 */
	virtual void setFrequencyHardware(uint32_t hz) = 0;
	/**Is called after data are written by the hardware. Will emit \c dataWritten signal.*/
	void onDataWritten();
	/**Is called after data is available in the hardware. Will emit \c dataAvailable signal.*/
	void onDataAvailable();
	/**
	 * @brief Is called if an error occurred by hardware read or write access. Will emit \c error signal.
	 * @param thrown A thrown error object.
	 */
	void onError(Error thrown);

private:
	/**I2c slave address for master an i2c device address for slave mode.*/
	uint8_t m_address = 0;
	/**Flag for hardware is busy reading or writing.*/
	bool m_isBusy = false;
	/**Buffering actual frame mode setting.*/
	CommunicationHardware::Frame m_frame = Frame::FirstAndLast;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::I2cMasterHardware;
};
} /* namespace semf */
#endif /* SEMF_COMMUNICATION_I2CMASTERHARDWARE_H_ */
