/**
 * @file softi2cmaster.h
 * @date 20.08.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_COMMUNICATION_SOFTI2CMASTER_H_
#define SEMF_COMMUNICATION_SOFTI2CMASTER_H_

#include <semf/app/system/timer.h>
#include <semf/communication/i2cmasterhardware.h>
#include <semf/system/gpio.h>
#include <semf/utils/core/signals/slot.h>

namespace semf
{
/**
 * @brief This class used two GPIOs (SCL and SDA) and a Timer and implements
 * a software I2C Master interface.
 *
 * If possible, use a hardware timer. The I2C frequency is half of the timer frequency.
 *
 * @attention The used GPIOs have to be configured in open drain mode and please use the
 * fastest possible GPIO clock setting.
 */
class SoftI2cMaster : public I2cMasterHardware
{
public:
	/**
	 * @brief Error IDs for this class. Error ID identify a unique error() / onError call (excluding transferring).
	 */
	enum class ErrorCode : uint8_t
	{
		StopConditionSetSda_NackError = 0
	};
	/**
	 * @brief Constructor.
	 * @attention I2C frequency is half of the \c timer frequency.
	 * @param scl GPIO to use as SCL output.
	 * @param sda GPIO to use as SDA output and input.
	 * @param timer Ideally use a hardware timer to handle the I2C timing.
	 */
	SoftI2cMaster(Gpio& scl, Gpio& sda, app::Timer& timer);
	explicit SoftI2cMaster(const SoftI2cMaster& other) = delete;
	virtual ~SoftI2cMaster() = default;

	void init() override;
	void deinit() override;
	void stopWrite() override;
	void stopRead() override;

protected:
	void writeHardware(const uint8_t data[], size_t size) override;
	void readHardware(uint8_t buffer[], size_t bufferSize) override;

private:
	// RESTART CONDITION
	/**Starts the restart condition.*/
	void resartConditionSetSdaResetScl();
	/**Slot for \c timeout signal following \c resartConditionSetSdaResetScl function.*/
	void resartConditionSetScl();

	// START CONDITION
	/**Starts the start condition.*/
	void startConditionResetSda();
	/**Slot for \c timeout signal following \c startConditionResetSda function.*/
	void startConditionResetScl();

	// WRITE BYTE
	/**Starts writing a byte.*/
	void writeByte();
	/**Sets the next data bit to the sda pin.*/
	void writeByteSetSdaDataBit();
	/**Slot for \c timeout signal following \c writeByteSetSdaDataBit function.*/
	void writeByteSetScl();
	/**
	 * Slot for \c timeout signal following \c writeByteSetScl function.
	 * After the byte is finished \c checkAcknolageSetSdaInput is called.
	 */
	void writeByteResetScl();

	// READ BYTE
	/**Starts reading a byte.*/
	void readByte();
	/**Reads the status of the sda pin.*/
	void readByteReadSdaDataBit();
	/**Slot for \c timeout signal following \c readByteReadSdaDataBit function.*/
	void readByteSetScl();
	/**
	 * Slot for \c timeout signal following \c readByteSetScl function.
	 * After the byte is finished \c setAcknolageSetSdaOutput is called.
	 */
	void readByteResetScl();

	// ACKNOLAGE
	/**Start checking the acknowledge bit for write operation. Setting sda as input.*/
	void checkAcknowledgeSetSdaInput();
	/**Slot for \c timeout signal following \c checkAcknowledgeSetSdaInput function.*/
	void checkAcknowledgeSetScl();
	/**Slot for \c timeout signal following \c checkAcknowledgeSetScl function.*/
	void checkAcknowledgeResetScl();
	/**Start setting the acknowledge bit for read operation. Setting sda as output.*/
	void setAcknowledgeSetSdaOutput();
	/**Slot for \c timeout signal following \c setAcknowledgeSetSdaOutput function.*/
	void setAcknowledgeSetScl();
	/**Slot for \c timeout signal following \c setAcknowledgeSetScl function.*/
	void setAcknowledgeResetScl();
	/**Finishing the acknowledge operation.*/
	void finishAcknowledge();
	/** Finishing the acknowledge operation for a write operation. Is called by \c finishAcknowledge.*/
	void finishAcknowledgeWriteOperation();
	/** Finishing the acknowledge operation for a read operation. Is called by \c finishAcknowledge.*/
	void finishAcknowledgeReadOperation();

	// STOP CONDITION
	/**Starts the stop condition.*/
	void stopConditionResetSda();
	/**Slot for \c timeout signal following \c stopConditionResetSda function.*/
	void stopConditionSetScl();
	/**
	 * @brief Slot for \c timeout signal following \c stopConditionSetScl function.
	 * @throws StopConditionSetSda_NackError If a NAK was received.
	 */
	void stopConditionSetSda();
	void setFrequency(uint32_t hz);

	/**Reference to used scl pin.*/
	Gpio& m_scl;
	/**Reference to used sda pin.*/
	Gpio& m_sda;
	/**Reference to the used timer.*/
	app::Timer& m_timer;
	/**Buffering read or write data array.*/
	uint8_t* m_data = nullptr;
	/**Buffering read or write data array size.*/
	size_t m_dataSize = 0;
	/**Index if buffer position.*/
	size_t m_dataIndex = 0;
	/**Buffering the momentary read or write byte.*/
	uint8_t m_activeByte = 0;
	/**Flag for handling address write before data read or write operation.*/
	bool m_writingAddress = false;
	/**Bit index of momentary byte.*/
	int8_t m_bitIndex = 7;
	/**Buffering the frame mode for doing start / restart condition correctly.*/
	CommunicationHardware::Frame m_lastFrame = CommunicationHardware::Frame::FirstAndLast;
	/**
	 * Buffering if the last operation was a write operation for doing a restart condition
	 * by doing a read after a write operation.
	 */
	bool m_lastOperationWasWrite = false;
	/**Buffering acknowledge bit. Bit is invers.*/
	bool m_acknowledgeBit = false;
	/**Flag for acknowledge error occurred after sending a byte.*/
	bool m_acknowledgeError = false;
	/**Slot for timer's timeout signal.*/
	SEMF_SLOT(m_timemoutSlot, SoftI2cMaster, *this, resartConditionSetScl);
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::SoftI2cMaster;
};
} /* namespace semf */
#endif /* SEMF_COMMUNICATION_SOFTI2CMASTER_H_ */
