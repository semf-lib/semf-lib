/**
 * @file spimasterhardware.cpp
 * @date 18.07.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 * 			  For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_COMMUNICATION_SPIMASTERHARDWARE_H_
#define SEMF_COMMUNICATION_SPIMASTERHARDWARE_H_

#include <semf/communication/communicationhardwaresynchronousmaster.h>
#include <semf/communication/spi.h>

namespace semf
{
/**
 * @brief Class for using SPI hardware in master mode.
 */
class SpiMasterHardware : public Spi, public CommunicationHardwareSynchronousMaster
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
		SetFormat_IsBusy,
		StopWrite_IsReadOnly,
		StopRead_IsWriteOnly,
		WriteRead_IsBusy,
		WriteRead_WriteDataIsNullptr,
		WriteRead_ReadBufferIsNullptr,
		WriteRead_SizeIsZero,
		SetChipSelectPin_IsBusy
	};

	SpiMasterHardware() = default;
	/**
	 * Constructor
	 * @param chipSelectPin Chip select pin for choosing the target device.
	 */
	explicit SpiMasterHardware(Gpio& chipSelectPin);
	virtual ~SpiMasterHardware() = default;

	/**
	 * @copydoc CommunicationHardwareSynchronousMaster::write()
	 * @throws Write_IsBusy If this is busy.
	 * @throws Write_DataIsNullptr If data is nullptr.
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
	void setFrame(CommunicationHardware::Frame frame) override;
	/**
	 * @copydoc Spi::setFormat()
	 * @throws SetFormat_IsBusy If this is busy
	 */
	void setFormat(uint8_t bits, TransmissionMode transmission, WireMode wire) override;
	/**
	 * @copydoc CommunicationHardwareSynchronousMaster::stopWrite()
	 * @throws StopWrite_IsReadOnly If master is configured as read only.
	 */
	void stopWrite() override;
	/**
	 * @copydoc CommunicationHardwareSynchronousMaster::stopRead()
	 * @throws StopRead_IsWriteOnly If master is configured as write only.
	 */
	void stopRead() override;
	/**
	 * @copydoc Spi::writeRead()
	 * @throws WriteRead_IsBusy If this is busy.
	 * @throws WriteRead_WriteDataIsNullptr If writeData is nullptr.
	 * @throws WriteRead_ReadBufferIsNullptr If readBuffer is nullptr.
	 * @throws Write_Read_Size_Is_Zero If size is zero.
	 */
	void writeRead(const uint8_t writeData[], uint8_t readBuffer[], size_t size) override;
	/**
	 * @brief Sets a new chip select pin read and write operations.
	 * @param chipSelectPin Digital out chip select pin.
	 * @throws SetChipSelectPin_IsBusy If this is busy.
	 */
	virtual void setChipSelectPin(Gpio& chipSelectPin);
	/**Enables chip select for read or write operation, if chip select was set in the past.*/
	void enableChipSelect();
	/**Disables chip select after read or write operation, if chip select was set in the past.*/
	void disableChipSelect();
	/**
	 * @brief Sets the speed.
	 * @param hz Frequency in hz.
	 */
	virtual void setFrequency(uint32_t hz) = 0;

protected:
	/**
	 * @brief Sets the busy flag.
	 * @note in the functions \c write(), \c read() and \c writeRead()
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
	 * @brief Hardware will write data
	 * @param data data array
	 * @param dataSize data array size
	 */
	virtual void writeHardware(const uint8_t data[], size_t dataSize) = 0;
	/**
	 * @brief Hardware will read data
	 * @param buffer data array
	 * @param bufferSize data array size
	 */
	virtual void readHardware(uint8_t buffer[], size_t bufferSize) = 0;
	/**
	 * @brief Hardware will read and write data parallel
	 * @param writeData memory where to write from
	 * @param readBuffer memory where to store read data
	 * @param size data array size
	 */
	virtual void writeReadHardware(const uint8_t writeData[], uint8_t readBuffer[], size_t size) = 0;
	/**
	 * @brief Configures the hardware for insuring the given configuration.
	 * @param bits Number of bits per frame.
	 * @param transmission Transmission mode (polarity and phase setting).
	 * @param wire Wire transmission setting.
	 */
	virtual void setFormatHardware(uint8_t bits, TransmissionMode transmission, WireMode wire) = 0;
	/**Forces the hardware to stop a writing process.*/
	virtual void stopWriteHardware() = 0;
	/**Forces the hardware to stop a reading process*/
	virtual void stopReadHardware() = 0;
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
	/**
	 * @brief Sets the transmission mode.
	 * @param mode The new transmission mode.
	 */
	void setTransmissionMode(TransmissionMode mode);
	/**
	 * @brief Sets the wire mode.
	 * @param mode The new wire mode:
	 */
	void setWireMode(WireMode mode);

private:
	/**Chip select pin*/
	Gpio* m_chipSelectPin = nullptr;
	/**Flag for hardware is busy reading or writing.*/
	bool m_isBusy = false;
	/**Buffering actual frame mode setting.*/
	CommunicationHardware::Frame m_frame = Frame::FirstAndLast;
	/**transmission mode*/
	TransmissionMode m_transmission;
	/**wirering configuration*/
	WireMode m_wire;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::SpiMasterHardware;
};
} /* namespace semf */
#endif /* SEMF_COMMUNICATION_SPIMASTERHARDWARE_H_ */
