/**
 * @file onewiremaster.h
 * @date Jul 19, 2024
 * @author cr, vb
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_COMMUNICATION_ONEWIREMASTER_H_
#define SEMF_COMMUNICATION_ONEWIREMASTER_H_

#include <semf/communication/onewire.h>
#include <semf/communication/communicationhardwareasynchronous.h>
#include <semf/utils/core/signals/slot.h>
#include <bitset>

namespace semf
{
/*
 * @brief Abstract class for all types of OneWire masters.
 */
class OneWireMaster : public CommunicationHardwareAsynchronous, public OneWire
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
		Reset_IsBusy
	};

	OneWireMaster() = default;
	explicit OneWireMaster(const OneWireMaster& other) = delete;
	virtual ~OneWireMaster() = default;

	void init() override;
	void deinit() override;
	/**
	 * @brief Sending Reset operation to 1-Wire. Emits \c presenceRecieved signal On receiving presence from slaves
	 */
	void sendResetCondition();
	void write(const uint8_t data[], size_t dataSize) override;
	void read(uint8_t buffer[], size_t bufferSize) override;
	void stopWrite() override;
	bool isBusyReading() const override;
	bool isBusyWriting() const override;
	void stopRead() override;
	/**
	 * @brief Emitted when the reset condition was successful. This means at least one slave responded to the reset operation.
	 * @param presence \li True if a slave responded.
	 * 		  		   \li False if no slave responded.
	 */
	Signal<bool> presenceDetected;

protected:
	/**
	 * @brief Sets a state for m_isBusyReading flag
	 */
	void setIsBusyReading(bool isBusy);
	/**
	 * @brief Sets a state for m_isBusyWriting flag
	 */
	void setIsBusyWriting(bool isBusy);

private:
	/**
	 * @brief Called when a single bit has been written. Starts writing the next bit or calls dataWritten if data was fully written.
	 */
	void onBitWritten();
	/**
	 * @brief Called when a single bit has been read. Starts reading the next bit or calls dataRead if buffer is full.
	 * @param bit The read bits value.
	 */
	void onBitRead(bool bit);
	/**
	 * @brief Called when the reset operation has finished.
	 * @param slavePresence Determines the presence of slaves on the line.
	 */
	void onResetted(bool slavePresence);

	/**
	 * @brief Read the next bit to write from the current write buffer. Increments the \c m_currentWriteBit.
	 * @return The next bit to write.
	 */
	bool prepareNextWriteBit();
	/**
	 * @brief Reset the current write buffer.
	 */
	void resetWrite();
	/**
	 * @brief Reset the current read buffer.
	 */
	void resetRead();

	/** Flag for indicating the master is busy reading. */
	bool m_isBusyReading = false;
	/** Flag for indicating the master is busy writing. */
	bool m_isBusyWriting = false;
	/** The current data to write. */
	const uint8_t* m_currentWriteData = nullptr;
	/** The size of current data to write. */
	size_t m_currentBufferSize = 0;
	/** The current bit index of the data to write. */
	size_t m_currentBit = 0;
	/** The current buffer to store read data in. */
	uint8_t* m_currentReadBuffer = nullptr;
	/** The byte currently being constructed by the read operation. */
	uint8_t m_currentlyReadByte = 0;
	/** Number of bits in a byte. */
	static constexpr uint8_t kBitsInByte = 8;

	SEMF_SLOT(m_onBitWrittenSlot, OneWireMaster, *this, onBitWritten);
	SEMF_SLOT(m_onBitReadSlot, OneWireMaster, *this, onBitRead, bool);
	SEMF_SLOT(m_onResettedSlot, OneWireMaster, *this, onResetted, bool);

	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = static_cast<Error::ClassID>(Error::ClassID::OneWireMaster);
};
}  // namespace semf

#endif  // SEMF_COMMUNICATION_ONEWIREMASTER_H_
