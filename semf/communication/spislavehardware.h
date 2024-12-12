/**
 * @file spislavehardware.h
 * @date 26.01.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_COMMUNICATION_SPISLAVEHARDWARE_H_
#define SEMF_COMMUNICATION_SPISLAVEHARDWARE_H_

#include <semf/communication/spi.h>
#include <semf/communication/communicationhardwaresynchronousslave.h>

namespace semf
{
/**
 * @brief Class for using spi hardware in slave mode. In order of being a SPI slave IO-operations will not get executed before the CS-Signal is LOW.
 * After calling an IO-operation successfully, the hardware state will change to busy while executing the IO call. The hardware state will change
 * to ready after IO-operation is done. When the IO-operation got executed successfully one of the signals \c dataWritten or \c dataAvailable gets
 * emitted. The signal handlers can start the next IO-operation as in all other classes deriving from \c CommunicationHardware.
 */
class SpiSlaveHardware : public Spi, public CommunicationHardwareSynchronousSlave
{
public:
	/**
	 * @brief Error IDs for this class. Error ID identify a unique error() / onError call (excluding transferring).
	 */
	enum class ErrorCode : uint8_t
	{
		Write_IsBusy = 0,
		Write_DataIsNullptr,
		Write_DataSizeIsZero,
		Read_IsBusy,
		Read_BufferIsNullptr,
		Read_BufferSizeIsZero,
		SetFormat_IsBusy,
		StopWrite_IsNotWriting,
		StopRead_IsNotReading,
		WriteRead_IsBusy,
		WriteRead_WriteDataIsNullptr,
		WriteRead_ReadBufferIsNullptr,
		WriteRead_SizeIsZero,
	};

	SpiSlaveHardware() = default;
	explicit SpiSlaveHardware(const SpiSlaveHardware& other) = delete;
	virtual ~SpiSlaveHardware() = default;

	/**
	 * @copydoc CommunicationHardwareSynchronousSlave::write()
	 * @throws Write_IsBusy If write is busy.
	 * @throws Write_DataIsNullptr If data is nullptr.
	 * @throws Write_DataSizeIsZero If dataSize is zero.
	 */
	void write(const uint8_t data[], size_t dataSize) override;
	/**
	 * @copydoc CommunicationHardwareSynchronousSlave::read()
	 * @throws Read_IsBusy If is busy.
	 * @throws Read_BufferIsNullptr If buffer is nullptr.
	 * @throws Read_BufferSizeIsZero If bufferSize is zero.
	 */
	void read(uint8_t buffer[], size_t bufferSize) override;
	bool isBusyReading() const override;
	bool isBusyWriting() const override;
	/**
	 * @copydoc CommunicationHardwareSynchronousSlave::stopRead()
	 * @throws StopRead_IsNotReading If slave is not currently reading.
	 */
	void stopRead() override;
	/**
	 * @copydoc CommunicationHardwareSynchronousSlave::stopWrite()
	 * @throws StopWrite_IsNotWriting If slave is not currently reading.
	 */
	void stopWrite() override;
	void setFrame(Frame frame) override;
	/**
	 * @copydoc Spi::writeRead()
	 * @throws WriteRead_IsBusy If this is busy.
	 * @throws WriteRead_WriteDataIsNullptr If writeData is a nullptr.
	 * @throws WriteRead_ReadBufferIsNullptr If readBuffer is a nullptr.
	 * @throws WriteRead_SizeIsZero If size is zero.
	 */
	void writeRead(const uint8_t writeData[], uint8_t readBuffer[], size_t size) override;
	/**
	 * @copydoc Spi::setFormat()
	 * @throws SetFormat_IsBusy If this is busy.
	 */
	void setFormat(uint8_t bits, TransmissionMode transmission, WireMode wire) override;

protected:
	/**
	 * @brief Error routine, emits the \c error signal.
	 * @param thrown A thrown error object.
	 */
	void onError(Error thrown);
	/**Data available routine, emits the \c dataAvailable signal.*/
	void onDataAvailable();
	/**Data written routine, emits the \c dataWritten signal.*/
	void onDataWritten();
	/**
	 * @brief Sets the flag \c m_isBusy.
	 * @param busy New value of the flag \c m_isBusyWriting.
	 */
	void setBusy(bool busy);
	/**
	 * @brief Accesses the spi hardware in order to write \c dataSize bytes of data.
	 * @param data Data location.
	 * @param dataSize Size of \c data.
	 */
	virtual void writeHardware(const uint8_t data[], size_t dataSize) = 0;
	/**
	 * @brief Accesses the spi hardware in order to read \c bufferSize bytes and stores them in \c buffer.
	 * @param buffer Buffer location.
	 * @param bufferSize Size of the \c buffer.
	 */
	virtual void readHardware(uint8_t buffer[], size_t bufferSize) = 0;
	/**
	 * @brief Accesses the spi hardware in order to perform a full duplex read/write.
	 * @param writeData Data to write.
	 * @param readBuffer Buffer to read.
	 * @param size Size of both buffers.
	 */
	virtual void writeReadHardware(const uint8_t writeData[], uint8_t readBuffer[], size_t size) = 0;
	/**Stops the hardware to read.*/
	virtual void stopReadHardware() = 0;
	/**Stop the hardware to write.*/
	virtual void stopWriteHardware() = 0;
	/**
	 * @brief Configures the hardware for insuring the given configuration.
	 * @param bits Number of bits per frame.
	 * @param transmission Transmission mode (polarity and phase setting).
	 * @param wire Wire transmission setting.
	 */
	virtual void setFormatHardware(uint8_t bits, TransmissionMode transmission, WireMode wire) = 0;
	/**
	 * @brief Sets the wire mode.
	 * @param mode The new wire mode.
	 */
	void setWireMode(WireMode mode);
	/**
	 * @brief Sets the transmission mode.
	 * @param mode The new transmission mode.
	 */
	void setTransmissionMode(TransmissionMode mode);

private:
	/**Flag for indicating reading hardware.*/
	bool m_isBusy = false;
	/**Wire mode.*/
	WireMode m_wire;
	/**Transmission mode.*/
	TransmissionMode m_transmission;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::SpiSlaveHardware;
};
} /* namespace semf */
#endif /* SEMF_COMMUNICATION_SPISLAVEHARDWARE_H_ */
