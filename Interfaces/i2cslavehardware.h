/**
 * @file i2cslavehardware.h
 *
 * @date 20.01.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef INTERFACES_I2CSLAVEHARDWARE_H_
#define INTERFACES_I2CSLAVEHARDWARE_H_

#include <Interfaces/i2c.h>
#include <Interfaces/communicationhardwaresynchronousslave.h>

namespace semf
{
/**
 * @brief This class can be used for using the I2C bus as a slave. Call \c startListening() for receiving I/O-requests from the master.
 *        You can handle them by connecting the signals \c readExpected and \c writeExpected and performing the I/O-calls accordingly.
 */
class I2cSlaveHardware : public I2c, public CommunicationHardwareSynchronousSlave
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
		SetAddress_IsBusy,
		SetFrequency_IsBusy,
		StartListening_IsBusy,
		StopListening_IsBusy
	};
	virtual ~I2cSlaveHardware() = default;
	/**
	 * @copydoc CommunicationHardwareSynchronousSlave::write()
	 * @throws Write_IsBusy If this is busy.
	 * @throws Write_DataIsNullptr If data is nullptr.
	 * @throws Write_DataSizeIsZero If dataSize is zero.
	 * @attention You should call \c startListening before calling this function. If \c write gets called from idle-state the behavior is unspecified!
	 */
	void write(const uint8_t data[], size_t dataSize);
	/**
	 * @copydoc CommunicationHardwareSynchronousSlave::read()
	 * @throws Read_IsBusy If this is busy.
	 * @throws Read_BufferIsNullptr If buffer is nullptr.
	 * @throws Read_BufferSizeIsZero If bufferSize is zero.
	 * @attention You should call \c startListening before calling this function. If \c write gets called from idle-state the behavior is unspecified!
	 */
	void read(uint8_t buffer[], size_t bufferSize);
	bool isBusyReading() const;
	bool isBusyWriting() const;
	void setFrame(CommunicationHardware::Frame frame);
	uint8_t address() const;
	/**
	 * @brief Sets the address of the slave.
	 * @param address new address of the slave.
	 * @throws SetAddress_IsBusy If this is busy.
	 */
	void setAddress(uint8_t address);
	/**
	 * @copydoc I2c::setFrequency()
	 * @throws SetFrequency_IsBusy If this is busy.
	 */
	void setFrequency(uint32_t hz);
	/**
	 * @brief Starts to react on getting addressed.
	 * @throws StartListening_IsBusy If this is busy.
	 */
	void startListening();
	/**
	 * @brief Quits reacting on getting addressed.
	 * @throws StopListening_IsBusy If this is listening.
	 */
	void stopListening();
	/**
	 * @brief Returns the listening flag.
	 * @return Listening flag.
	 */
	bool isListening() const;

	/**Gets emitted if the master requests to write.*/
	Signal<> readExpected;
	/**Gets emitted if the master requests to read.*/
	Signal<> writeExpected;

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
	 * @brief Hardware will react on getting addressed.
	 */
	virtual void startListeningHardware() = 0;
	/**
	 * @brief Hardware will ignore getting addressed.
	 */
	virtual void stopListeningHardware() = 0;
	/**
	 * @brief Sets the address of the i2c hardware. 
	 * @param address Address of the slave.
	 */
	virtual void setAddressHardware(uint8_t address) = 0;
	/**
	 * @brief Sets the listening flag.
	 * @param listening The new listening flag.
	 */
	void setListening(bool listening);
	/**
	 * @brief Configures the frequency of the bus.
	 * @param hz Frequency in hertz.
	 */
	virtual void setFrequencyHardware(uint32_t hz) = 0;
	/**
	 * @brief Is called after data are written by the hardware.
	 * Will emit \c dataWritten signal.
	 */
	void onDataWritten();

	/**
	 * @brief Is called after data is available in the hardware.
	 * Will emit \c dataAvailable signal.
	 */
	void onDataAvailable();

	/**
	 * @brief Is called if an error occurred by hardware read or write access.
	 * Will emit \c error signal.
	 * @param thrown A thrown error object.
	 */
	void onError(Error thrown);

private:
	/**Flag for hardware is busy reading or writing.*/
	bool m_isBusy = false;
	/**Buffering actual frame mode setting.*/
	CommunicationHardware::Frame m_frame = Frame::FirstAndLast;
	/**Flag for indicating if the slave is listening.*/
	bool m_listening = false;
	/**Address of the slave device.*/
	uint8_t m_address = 0;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::I2cSlaveHardware;
};

}  // namespace semf

#endif  // INTERFACES_I2CSLAVEHARDWARE_H_
