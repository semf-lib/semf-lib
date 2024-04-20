/**
 * @file stm32can.h
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32_STM32CAN_H_
#define SEMF_HARDWAREABSTRACTION_STM32_STM32CAN_H_

#include <semf/hardwareabstraction/stm32/stm32.h>
#include <semf/utils/core/queues/linkedqueue.h>

#if defined(STM32) && defined(HAL_CAN_MODULE_ENABLED)
#include <semf/communication/canhardware.h>
namespace semf
{
/**
 * @brief Class for using CAN with Stm32.
 * @attention For using the driver you need to call \c start().
 * @attention The implementation is for single CAN periphery usage.
 */
class Stm32Can : public CanHardware, public LinkedQueue<Stm32Can>::Node
{
public:
	/** Error codes for this class. Error ID identify a unique error() / onError call (excluding transferring).*/
	enum class ErrorCode : uint8_t
	{
		Init_HalError = 0,
		Init_HalBusy,
		Init_HalTimeout,
		Deinit_HalError,
		Deinit_HalBusy,
		Deinit_HalTimeout,
		SetFilter_IndexOutOfBounds,
		SetFilter_MessageIdInvalid,
		SetFilter_MessageIdMaskInvalid,
		EnterDeepPowerDownMode_SleepStateReadyListeningHalError,
		EnterDeepPowerDownMode_SleepStateReadyListeningHalBusy,
		EnterDeepPowerDownMode_SleepStateReadyListeningHalTimeout,
		EnterDeepPowerDownMode_InitStateResetSleepPendingHalError,
		EnterDeepPowerDownMode_InitStateResetSleepPendingHalBusy,
		EnterDeepPowerDownMode_InitStateResetSleepPendingHalTimeout,
		EnterDeepPowerDownMode_StartStateResetSleepPendingHalError,
		EnterDeepPowerDownMode_StartStateResetSleepPendingHalBusy,
		EnterDeepPowerDownMode_StartStateResetSleepPendingHalTimeout,
		EnterDeepPowerDownMode_SleepStateResetSleepPendingHalError,
		EnterDeepPowerDownMode_SleepStateResetSleepPendingHalBusy,
		EnterDeepPowerDownMode_SleepStateResetSleepPendingHalTimeout,
		ReleaseDeepPowerDownMode_WakeupStateSleepActiveHalError,
		ReleaseDeepPowerDownMode_WakeupStateSleepActiveHalBusy,
		ReleaseDeepPowerDownMode_WakeupStateSleepActiveHalTimeout,
		ReleaseDeepPowerDownMode_InitStateResetSleepPendingHalError,
		ReleaseDeepPowerDownMode_InitStateResetSleepPendingHalBusy,
		ReleaseDeepPowerDownMode_InitStateResetSleepPendingHalTimeout,
		ReleaseDeepPowerDownMode_StartStateResetSleepPendingHalError,
		ReleaseDeepPowerDownMode_StartStateResetSleepPendingHalBusy,
		ReleaseDeepPowerDownMode_StartStateResetSleepPendingHalTimeout,
		ReleaseDeepPowerDownMode_StartStateError,
		ReleaseDeepPowerDownMode_StartStateInvalid,
		IsrRead_NullpointerReadBuffer,
		IsrRead_FifoIdInvalid,
		IsrRead_HalError,
		IsrRead_HalBusy,
		IsrRead_HalTimeout,
		IsrError_Error,
		WriteHardware_InvalidMessageId,
		WriteHardware_HalError,
		WriteHardware_HalBusy,
		WriteHardware_HalTimeout,
		WriteHardware_NotStarted,
		RequestHardware_InvalidMessageId,
		RequestHardware_HalError,
		RequestHardware_HalBusy,
		RequestHardware_HalTimeout,
		RequestHardware_NotStarted,
		Start_HalTimeout,
		Start_HalNotInitialized,
		Stop_HalTimeout,
	};
	/**
	 * @brief Constructor.
	 * @param hwHandle can hardware interface
	 */
	explicit Stm32Can(CAN_HandleTypeDef& hwHandle);
	explicit Stm32Can(const Stm32Can& other) = delete;
	virtual ~Stm32Can() = default;

	/**
	 * @copydoc CanHardware::init()
	 * @throws Init_HalError If the ST-HAL stumbles upon an error.
	 * @throws Init_HalBusy If the ST-HAL is busy.
	 * @throws Init_HalTimeout If the ST-HAL times out.
	 */
	void init() override;
	/**
	 * @copydoc CanHardware::deinit()
	 * @throws Deinit_HalError If the ST-HAL stumbles upon an error.
	 * @throws Deinit_HalBusy If the ST-HAL is busy.
	 * @throws Deinit_HalTimeout If the ST-HAL times out.
	 */
	void deinit() override;
	void setReadBuffer(uint8_t buffer[], size_t bufferSize) override;
	void stopWrite() override;
	void stopRead() override;
	/**
	 * @brief Hardware driver will be started.
	 * @throws Start_HalTimeout If the ST-HAL times out.
	 * @throws Start_HalNotInitialized If the ST-HAL is not initialized.
	 */
	void start();
	/**
	 * @brief Hardware driver will be stopped.
	 * @throws Stop_HalTimeout If the ST-HAL times out.
	 */
	void stop();
	/**
	 * @brief Get the state of the start of the CAN driver.
	 * @return True if the driver already started.
	 */
	bool isStarted() const;
	uint32_t messageId() const override;
	void setMessageId(uint32_t id) override;
	void setFrequency(uint32_t hz) override;
	/**
	 * @copydoc CanHardware::setFilter()
	 * @attention Filter is tested for the normal can address. Filter implementation is on the link.
	 * @see https://schulz-m.github.io/2017/03/23/stm32-can-id-filter
	 * @throws SetFilter_IndexOutOfBounds If index is too high.
	 * @throws SetFilter_MessageIdInvalid If message id is invalid.
	 * @throws SetFilter_MessageIdMaskInvalid If the mask of the messageID is invalid.
	 */
	void setFilter(uint32_t filterBank, uint32_t messageId, uint32_t messageIdMask) override;
	/**
	 * @brief Go to deep power down mode.
	 * @throws EnterDeepPowerDownMode_SleepStateReadyListeningHalError If sleep returns a hal error.
	 * @throws EnterDeepPowerDownMode_SleepStateReadyListeningHalBusy If sleep returns a hal busy.
	 * @throws EnterDeepPowerDownMode_SleepStateReadyListeningHalTimeout If sleep returns a hal timeout.
	 * @throws EnterDeepPowerDownMode_InitStateResetSleepPendingHalError If init returns a hal error.
	 * @throws Enterdeeppowerdownmode_Initstateresetsleeppendinghalbusy If init returns a hal busy.
	 * @throws EnterDeepPowerDownMode_InitStateResetSleepPendingHalTimeout If init returns a hal timeout.
	 * @throws EnterDeepPowerDownMode_StartStateResetSleepPendingHalError If start returns a hal error.
	 * @throws EnterDeepPowerDownMode_StartStateResetSleepPendingHalBusy If start returns a hal busy.
	 * @throws EnterDeepPowerDownMode_StartStateResetSleepPendingHalTimeout If start returns a hal timeout.
	 * @throws EnterDeepPowerDownMode_SleepStateResetSleepPendingHalError If sleep returns a hal error.
	 * @throws EnterDeepPowerDownMode_SleepStateResetSleepPendingHalBusy If sleep returns a hal busy.
	 * @throws EnterDeepPowerDownMode_SleepStateResetSleepPendingHalTimeout If sleep returns a hal timeout.
	 */
	void enterDeepPowerDownMode();
	/**
	 * @brief Go to active mode.
	 * @throws ReleaseDeepPowerDownMode_WakeupStateSleepActiveHalError If wakeup returns a hal error.
	 * @throws ReleaseDeepPowerDownMode_WakeupStateSleepActiveHalBusy If wakeup returns a hal busy.
	 * @throws ReleaseDeepPowerDownMode_WakeupStateSleepActiveHalTimeout If wakeup returns a hal timeout.
	 * @throws ReleaseDeepPowerDownMode_InitStateResetSleepPendingHalError If init returns a hal error.
	 * @throws ReleaseDeepPowerDownMode_InitStateResetSleepPendingHalBusy If init returns a hal busy.
	 * @throws ReleaseDeepPowerDownMode_InitStateResetSleepPendingHalTimeout If init returns a hal timeout.
	 * @throws ReleaseDeepPowerDownMode_StartStateResetSleepPendingHalError If start returns a hal error.
	 * @throws ReleaseDeepPowerDownMode_StartStateResetSleepPendingHalBusy If start returns a hal busy.
	 * @throws ReleaseDeepPowerDownMode_StartStateResetSleepPendingHalTimeout If start returns a hal timeout.
	 * @throws ReleaseDeepPowerDownMode_StartStateError If illegal state got reached.
	 * @throws ReleaseDeepPowerDownMode_StartStateInvalid If illegal state got reached.
	 */
	void releaseDeepPowerDownMode();
	/**
	 * @brief Get the list with all cans.
	 * @return The list with all cnas.
	 */
	static LinkedQueue<Stm32Can>* queue();
	/**
	 * @brief System-wide interrupt service routine for can receive.
	 * @param can Hardware handler.
	 * @param fifoId Id message has read into.
	 */
	static void systemIsrRead(CAN_HandleTypeDef& can, uint8_t fifoId);
	/**
	 * @brief System-wide interrupt service routine for can transmit.
	 * @param can Hardware handler.
	 * @param mailboxId Id of mailbox where message was written from.
	 */
	static void systemIsrWritten(CAN_HandleTypeDef& can, uint8_t mailboxId);
	/**
	 * @brief System-wide interrupt service routine for can error.
	 * @param can Hardware handler.
	 */
	static void systemIsrError(CAN_HandleTypeDef& can);
	/**
	 * @brief Saves a received message and sends a \c readyRead or \c readyRequested signal.
	 * @param can Pointer to CAN hardware handler.
	 * @param fifoId FIFO ID of the received message.
	 * @throws IsrRead_NullpointerReadBuffer Read data is not set.
	 * @throws IsrRead_FifoIdInvalid If FIFO-id is invalid.
	 * @throws IsrRead_HalError If ST-HAL returns hal error.
	 * @throws IsrRead_HalBusy If ST-HAL returns hal busy.
	 * @throws IsrRead_HalTimeout If ST-HAL returns hal timeout.
	 */
	void isrRead(CAN_HandleTypeDef& can, uint8_t fifoId);
	/**
	 * @brief Sending a \c readyWritten signal.
	 * @param can Pointer to CAN hardware handler.
	 * @param mailboxId Id of mailbox where message was written from.
	 */
	void isrWritten(CAN_HandleTypeDef& can, uint8_t mailboxId);
	/**
	 * @brief Sending a \c error signal.
	 * @param can Pointer to CAN hardware handler.
	 */
	void isrError(CAN_HandleTypeDef& can);

protected:
	/**
	 * @copydoc CanHardware::writeHardware()
	 * @throws WriteHardware_InvalidMessageId If message id is invalid.
	 * @throws WriteHardware_HalError If ST-HAL returns HAL error.
	 * @throws WriteHardware_HalBusy If ST-HAL returns HAL busy.
	 * @throws WriteHardware_HalTimeout If ST-HAL returns HAL timeout.
	 * @throws WriteHardware_NotStarted If ST-HAL not started.
	 */
	void writeHardware(const uint8_t data[], size_t dataSize) override;
	/**
	 * @copydoc CanHardware::requestHardware()
	 * @throws RequestHardware_InvalidMessageId If message id is invalid.
	 * @throws RequestHardware_HalError If ST-HAL returns HAL error.
	 * @throws RequestHardware_HalBusy If ST-HAL returns HAL busy.
	 * @throws RequestHardware_HalTimeout If ST-HAL returns HAL timeout.
	 * @throws RequestHardware_NotStarted If ST-HAL not started.
	 */
	void requestHardware() override;

private:
	/**List for all cans*/
	static LinkedQueue<Stm32Can> m_queue;
	/**CAN hardware handler.*/
	CAN_HandleTypeDef* m_hwHandle = nullptr;
	/**Maximum number of existing hardware filter banks, each consists of 2 filters.*/
	static const uint8_t m_numOfFilterBanks = 28;
	/**CAN Tx message header structure definition.*/
	CAN_TxHeaderTypeDef m_txHeader;
	/**CAN Rx message header structure definition.*/
	CAN_RxHeaderTypeDef m_rxHeader;
	/**Mailbox on which the message is sent.*/
	uint32_t m_usedTxMailbox = 0;
	/**Buffer for write data.*/
	const uint8_t* m_writeData = nullptr;
	/**Size of write data buffer.*/
	size_t m_writeDataSize = 0;
	/**Index of momentary byte to write.*/
	size_t m_writeDataIndex = 0;
	/**Internal buffer for request (STM HAL Workarround).*/
	uint8_t m_requestData[8] = {0};
	/**Buffer for read data.*/
	uint8_t* m_readData = nullptr;
	/**Size of read data buffer.*/
	size_t m_readDataSize = 0;
	/**Message id for write.*/
	uint32_t m_messageIdWrite = 0;
	/**Message id for read.*/
	uint32_t m_messageIdRead = 0;
	/**Flag for hardware is started.*/
	bool m_isStarted = false;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32Can;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32_STM32CAN_H_ */
