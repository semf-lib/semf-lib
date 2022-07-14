/**
 * @file stm32can.cpp
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <HardwareAbstraction/Stm32/stm32can.h>
#include <Core/debug.h>

#if defined(STM32) && defined(HAL_CAN_MODULE_ENABLED)

extern "C"
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef* can)
{
	semf::Stm32Can::systemIsrWritten(*can, 0);
}
extern "C"
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef* can)
{
	semf::Stm32Can::systemIsrWritten(*can, 1);
}
extern "C"
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef* can)
{
	semf::Stm32Can::systemIsrWritten(*can, 3);
}
extern "C"
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef* can)
{
	semf::Stm32Can::systemIsrError(*can);
}
extern "C"
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* can)
{
	semf::Stm32Can::systemIsrRead(*can, 0);
}
extern "C"
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef* can)
{
	semf::Stm32Can::systemIsrRead(*can, 1);
}
namespace semf
{
LinkedQueue<Stm32Can> Stm32Can::m_queue;

Stm32Can::Stm32Can(CAN_HandleTypeDef& hwHandle)
	:m_hwHandle(&hwHandle)
{
	queue()->push(*this);
}

void Stm32Can::init()
{
	SEMF_INFO("init");
	HAL_StatusTypeDef state = HAL_CAN_Init(m_hwHandle);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_HalTimeout)));
		}
		return;
	}
}

void Stm32Can::deinit()
{
	SEMF_INFO("deinit");
	HAL_StatusTypeDef state = HAL_CAN_DeInit(m_hwHandle);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("dhal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_HalTimeout)));
		}
		return;
	}
	m_isStarted = false;
}

void Stm32Can::setReadBuffer(uint8_t buffer[], size_t bufferSize)
{
	m_readData = buffer;
	m_readDataSize = bufferSize;
}

void Stm32Can::stopWrite()
{
	SEMF_INFO("not implemented");
	// TODO(FS) not implemented
	return;
}

void Stm32Can::stopRead()
{
	SEMF_INFO("not implemented");
	// TODO(FS) not implemented
	return;
}

bool Stm32Can::isStarted() const
{
	return m_isStarted;
}

uint32_t Stm32Can::messageId() const
{
	return m_messageIdRead;
}

void Stm32Can::setMessageId(uint32_t id)
{
	SEMF_INFO("message Id: %u", id);
	m_messageIdWrite = id;
}

void Stm32Can::setFrequency(uint32_t hz)
{
	(void)hz;
	SEMF_INFO("not implemented");
	// TODO(FS) not implemented
	return;
}

void Stm32Can::setFilter(uint32_t filterBank, uint32_t messageId, uint32_t messageIdMask)
{
	SEMF_INFO("index: %u, messageId: %u, messageIdMask: %u", index, messageId, messageIdMask);
	CAN_FilterTypeDef rxFilter;

	if (filterBank >= m_numOfFilterBanks)
	{
		onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetFilter_IndexOutOfBounds)));
		return;
	}

	rxFilter.FilterBank = filterBank;
	rxFilter.FilterMode = CAN_FILTERMODE_IDMASK;  // alternative CAN_FILTERMODE_IDLIST
	rxFilter.FilterFIFOAssignment = CAN_RX_FIFO0;
	rxFilter.FilterScale = CAN_FILTERSCALE_32BIT;
	rxFilter.FilterActivation = ENABLE;

	if (messageIdMask <= 0x1FFFFFFF)
	{
		rxFilter.FilterIdHigh = ((messageId << 5)  | (messageId >> (32 - 5))) & 0xFFFF;  // STID[10:0] & EXTID[17:13]
		rxFilter.FilterIdLow = (messageId >> (11 - 3)) & 0xFFF8;  // EXID[12:5] & 3 Reserved bits

		rxFilter.FilterMaskIdHigh = ((messageIdMask << 5)  | (messageIdMask >> (32 - 5))) & 0xFFFF;
		rxFilter.FilterMaskIdLow = (messageIdMask >> (11 - 3)) & 0xFFF8;
	}
	else
	{
		SEMF_ERROR("invalid message id mask: %u", messageIdMask);
		onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetFilter_MessageIdMaskInvalid)));
		return;
	}
	HAL_CAN_ConfigFilter(m_hwHandle, &rxFilter);
}

void Stm32Can::enterDeepPowerDownMode()
{
	SEMF_INFO("enter deep power down mode");
	HAL_CAN_StateTypeDef canstate = HAL_CAN_GetState(m_hwHandle);
	HAL_StatusTypeDef state;
	switch (canstate)
	{
	case HAL_CAN_STATE_READY:
	case HAL_CAN_STATE_LISTENING:  // is already started
		SEMF_INFO("state is ready or listening");
		state = HAL_CAN_RequestSleep(m_hwHandle);
		if (state != HAL_OK)
		{
			if (state == HAL_ERROR)
			{
				SEMF_ERROR("request sleep hal error");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::EnterDeepPowerDownMode_SleepStateReadyListeningHalError)));
			}
			else if (state == HAL_BUSY)
			{
				SEMF_ERROR("request sleep hal busy");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::EnterDeepPowerDownMode_SleepStateReadyListeningHalBusy)));
			}
			else if (state == HAL_TIMEOUT)
			{
				SEMF_ERROR("request sleep hal timeout");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::EnterDeepPowerDownMode_SleepStateReadyListeningHalTimeout)));
			}
			return;
		}
		break;

	case HAL_CAN_STATE_SLEEP_ACTIVE:
		SEMF_INFO("state is sleep active");
		break;

	case HAL_CAN_STATE_RESET:
	case HAL_CAN_STATE_SLEEP_PENDING:
		SEMF_INFO("state is reset or sleep pending");
		state = HAL_CAN_Init(m_hwHandle);
		if (state != HAL_OK)
		{
			if (state == HAL_ERROR)
			{
				SEMF_ERROR("init hal error");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::EnterDeepPowerDownMode_InitStateResetSleepPendingHalError)));
			}
			else if (state == HAL_BUSY)
			{
				SEMF_ERROR("init hal busy");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::EnterDeepPowerDownMode_InitStateResetSleepPendingHalBusy)));
			}
			else if (state == HAL_TIMEOUT)
			{
				SEMF_ERROR("init hal timeout");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::EnterDeepPowerDownMode_InitStateResetSleepPendingHalTimeout)));
			}
			return;
		}
		state = HAL_CAN_Start(m_hwHandle);
		if (state != HAL_OK)
		{
			if (state == HAL_ERROR)
			{
				SEMF_ERROR("start hal error");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::EnterDeepPowerDownMode_StartStateResetSleepPendingHalError)));
			}
			else if (state == HAL_BUSY)
			{
				SEMF_ERROR("start hal busy");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::EnterDeepPowerDownMode_StartStateResetSleepPendingHalBusy)));
			}
			else if (state == HAL_TIMEOUT)
			{
				SEMF_ERROR("start hal timeout");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::EnterDeepPowerDownMode_StartStateResetSleepPendingHalTimeout)));
			}
			return;
		}
		state = HAL_CAN_RequestSleep(m_hwHandle);
		if (state != HAL_OK)
		{
			if (state == HAL_ERROR)
			{
				SEMF_ERROR("request sleep hal error");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::EnterDeepPowerDownMode_SleepStateResetSleepPendingHalError)));
			}
			else if (state == HAL_BUSY)
			{
				SEMF_ERROR("request sleep hal busy");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::EnterDeepPowerDownMode_SleepStateResetSleepPendingHalBusy)));
			}
			else if (state == HAL_TIMEOUT)
			{
				SEMF_ERROR("request sleep hal timeout");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::EnterDeepPowerDownMode_SleepStateResetSleepPendingHalTimeout)));
			}
			return;
		}
		break;
	case HAL_CAN_STATE_ERROR:
		SEMF_INFO("state error");
		break;
	default:
		SEMF_INFO("invalid state");
		break;
	}
}

void Stm32Can::releaseDeepPowerDownMode()
{
	SEMF_INFO("release deep power down mode");

	HAL_CAN_StateTypeDef canstate = HAL_CAN_GetState(m_hwHandle);
	HAL_StatusTypeDef state;
	switch (canstate)
	{
	case HAL_CAN_STATE_READY:
	case HAL_CAN_STATE_LISTENING:  // is already started
		SEMF_INFO("state is ready or listening");
		break;

	case HAL_CAN_STATE_SLEEP_ACTIVE:
		state = HAL_CAN_WakeUp(m_hwHandle);
		if (state != HAL_OK)
		{
			if (state == HAL_ERROR)
			{
				SEMF_ERROR("hal error");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReleaseDeepPowerDownMode_WakeupStateSleepActiveHalError)));
			}
			else if (state == HAL_BUSY)
			{
				SEMF_ERROR("hal busy");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReleaseDeepPowerDownMode_WakeupStateSleepActiveHalBusy)));
			}
			else if (state == HAL_TIMEOUT)
			{
				SEMF_ERROR("hal timeout");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReleaseDeepPowerDownMode_WakeupStateSleepActiveHalTimeout)));
			}
			return;
		}
		break;

	case HAL_CAN_STATE_RESET:
	case HAL_CAN_STATE_SLEEP_PENDING:
		SEMF_INFO("state is reset or sleep pending");
		state = HAL_CAN_Init(m_hwHandle);
		if (state != HAL_OK)
		{
			if (state == HAL_ERROR)
			{
				SEMF_ERROR("init hal error");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReleaseDeepPowerDownMode_InitStateResetSleepPendingHalError)));
			}
			else if (state == HAL_BUSY)
			{
				SEMF_ERROR("init hal busy");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReleaseDeepPowerDownMode_InitStateResetSleepPendingHalBusy)));
			}
			else if (state == HAL_TIMEOUT)
			{
				SEMF_ERROR("init hal timeout");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReleaseDeepPowerDownMode_InitStateResetSleepPendingHalTimeout)));
			}
			return;
		}
		state = HAL_CAN_Start(m_hwHandle);
		if (state != HAL_OK)
		{
			if (state == HAL_ERROR)
			{
				SEMF_ERROR("start hal error");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReleaseDeepPowerDownMode_StartStateResetSleepPendingHalError)));
			}
			else if (state == HAL_BUSY)
			{
				SEMF_ERROR("start hal busy");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReleaseDeepPowerDownMode_StartStateResetSleepPendingHalBusy)));
			}
			else if (state == HAL_TIMEOUT)
			{
				SEMF_ERROR("start hal timeout");
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReleaseDeepPowerDownMode_StartStateResetSleepPendingHalTimeout)));
			}
			return;
		}
		break;
	case HAL_CAN_STATE_ERROR:
		SEMF_ERROR("state error");
		onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReleaseDeepPowerDownMode_StartStateError)));
		break;
	default:
		SEMF_ERROR("state invalid");
		onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReleaseDeepPowerDownMode_StartStateInvalid)));
		break;
	}
}

LinkedQueue<Stm32Can>* Stm32Can::queue()
{
	static LinkedQueue<Stm32Can> queue;
	return &queue;
}

void Stm32Can::systemIsrRead(CAN_HandleTypeDef& can, uint8_t fifoId)
{
	for (Stm32Can& i : *(queue()))
		i.isrRead(can, fifoId);
}

void Stm32Can::systemIsrWritten(CAN_HandleTypeDef& can, uint8_t mailboxId)
{
	for (Stm32Can& i : *(queue()))
		i.isrWritten(can, mailboxId);
}

void Stm32Can::systemIsrError(CAN_HandleTypeDef& can)
{
	for (Stm32Can& i : *(queue()))
		i.isrError(can);
}

void Stm32Can::isrRead(CAN_HandleTypeDef& can, uint8_t fifoId)
{
	if (&can != m_hwHandle)
		return;

	if (m_readData == nullptr)
	{
		SEMF_ERROR("invalid read data");
		onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::IsrRead_NullpointerReadBuffer)));
		return;
	}

	SEMF_INFO("data available");
	if (fifoId > CAN_RX_FIFO1)
	{
		SEMF_ERROR("invalid fifo id: %u", fifoId);
		onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::IsrRead_FifoIdInvalid)));
		return;
	}

	HAL_StatusTypeDef state = HAL_CAN_GetRxMessage(m_hwHandle, fifoId == 0 ? CAN_RX_FIFO0 : CAN_RX_FIFO1,
			&m_rxHeader, m_readData/*[m_readDataIndex]*/);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::IsrRead_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::IsrRead_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::IsrRead_HalTimeout)));
		}
		return;
	}

	if (m_rxHeader.IDE == CAN_ID_EXT)
		m_messageIdRead = m_rxHeader.ExtId;
	else
		m_messageIdRead = m_rxHeader.StdId;

	if (m_rxHeader.RTR == CAN_RTR_REMOTE)
		onDataRequested();
	else
		onDataAvailable();
}

void Stm32Can::isrWritten(CAN_HandleTypeDef& can, uint8_t mailboxId)
{
	(void)mailboxId;
	if (&can != m_hwHandle)
		return;

	m_writeDataIndex += m_txHeader.DLC;
	if (m_writeDataIndex >= m_writeDataSize)
		onDataWritten();
	else
		writeHardware(&m_writeData[m_writeDataIndex], m_writeDataSize - m_writeDataIndex);
}

void Stm32Can::isrError(CAN_HandleTypeDef& can)
{
	if (&can != m_hwHandle)
		return;

	SEMF_ERROR("isr error");
	onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::IsrError_Error)));
}

void Stm32Can::writeHardware(const uint8_t data[], size_t dataSize)
{
	SEMF_INFO("write data: %p, size %u", data, dataSize);
	if (!m_isStarted)
	{
		SEMF_ERROR("not started");
		onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteHardware_NotStarted)));
	}
	m_writeData = data;
	m_writeDataSize = dataSize;
	m_writeDataIndex = 0;
	if (m_messageIdWrite <= 0x7FF)
	{
		m_txHeader.IDE = CAN_ID_STD;
		m_txHeader.StdId = m_messageIdWrite & 0x7FF;
	}
	else if (m_messageIdWrite <= 0x1FFFFFFF)
	{
		m_txHeader.IDE = CAN_ID_EXT;
		m_txHeader.ExtId = m_messageIdWrite & 0x1FFFFFFF;
	}
	else
	{
		SEMF_ERROR("invalid message id: %u", m_messageIdWrite);
		onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteHardware_InvalidMessageId)));
		return;
	}

	m_txHeader.DLC = dataSize > 8 ? 8 : dataSize;
	m_txHeader.RTR = CAN_RTR_DATA;
	m_txHeader.TransmitGlobalTime = FunctionalState::DISABLE;

	HAL_StatusTypeDef state = HAL_CAN_AddTxMessage(m_hwHandle, &m_txHeader, const_cast<uint8_t*>(data), &m_usedTxMailbox);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteHardware_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteHardware_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteHardware_HalTimeout)));
		}
		return;
	}
}

void Stm32Can::requestHardware()
{
	SEMF_INFO("request data");
	if (!m_isStarted)
	{
		SEMF_ERROR("not started");
		onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::RequestHardware_NotStarted)));
	}

	// txHeader configuration
	if (m_messageIdWrite <= 0x7FF)  // standard
	{
		m_txHeader.IDE = CAN_ID_STD;  // Identifier extension bit
		m_txHeader.StdId = m_messageIdWrite & 0x7FF;
	}
	else if (m_messageIdWrite <= 0x1FFFFFFF)  // fill up the extended id
	{
		m_txHeader.IDE = CAN_ID_EXT;  // Identifier extension bit
		m_txHeader.ExtId = m_messageIdWrite & 0x1FFFFFFF;
	}
	else
	{
		SEMF_ERROR("invalid message id: %u", m_messageIdWrite);
		onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::RequestHardware_InvalidMessageId)));
		return;
	}

	m_txHeader.DLC = 0;  // Data size code
	m_txHeader.RTR = CAN_RTR_REMOTE;  // Remote transmission request
	// Specifies whether the timestamp counter value captured on
	// start of frame transmission within the data field in the frame
	m_txHeader.TransmitGlobalTime = FunctionalState::DISABLE;

	HAL_StatusTypeDef state = HAL_CAN_AddTxMessage(m_hwHandle, &m_txHeader, m_requestData, &m_usedTxMailbox);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::RequestHardware_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::RequestHardware_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::RequestHardware_HalTimeout)));
		}
		return;
	}
}

void Stm32Can::start()
{
	HAL_StatusTypeDef state = HAL_CAN_Start(m_hwHandle);

	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Start_HalTimeout)));
		}
		return;
	}

	HAL_StatusTypeDef notificationState =  HAL_CAN_ActivateNotification(m_hwHandle,
		CAN_IT_RX_FIFO0_MSG_PENDING |
		CAN_IT_RX_FIFO1_MSG_PENDING |
		CAN_IT_TX_MAILBOX_EMPTY);
	if (notificationState != HAL_OK)
	{
		if (notificationState == HAL_ERROR)
		{
			SEMF_ERROR("hal not initialized");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Start_HalNotInitialized)));
		}
		return;
	}
	m_isStarted = true;
}

void Stm32Can::stop()
{
	HAL_StatusTypeDef state = HAL_CAN_Stop(m_hwHandle);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Stop_HalTimeout)));
		}
		return;
	}
	m_isStarted = false;
}

} /* namespace semf */
#endif
