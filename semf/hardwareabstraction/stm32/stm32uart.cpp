/**
 * @file stm32uart.cpp
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/hardwareabstraction/stm32/stm32uart.h>
#include <semf/utils/core/debug.h>

#if defined(STM32) && defined(HAL_UART_MODULE_ENABLED)
extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
{
	semf::Stm32Uart::systemIsrWritten(*huart);
}
extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
	semf::Stm32Uart::systemIsrRead(*huart);
}

namespace semf
{
LinkedQueue<Stm32Uart> Stm32Uart::m_queue;
Stm32Uart::Stm32Uart(UART_HandleTypeDef& hwHandle)
: m_hwHandle(&hwHandle),
  m_direction{Direction::RxTx},
  m_wireMode{WireMode::Duplex}
{
	queue()->push(*this);
	setDirection(m_direction);
}

void Stm32Uart::init()
{
	SEMF_INFO("init");
	setWireMode(m_wireMode);
}

void Stm32Uart::deinit()
{
	SEMF_INFO("deinit");
	HAL_StatusTypeDef state = HAL_UART_DeInit(m_hwHandle);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Deinit_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Deinit_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Deinit_HalTimeout)));
		}
		return;
	}
}

void Stm32Uart::stopWrite()
{
	SEMF_INFO("stop write");
	HAL_StatusTypeDef state = HAL_UART_AbortTransmit(m_hwHandle);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopWrite_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopWrite_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopWrite_HalTimeout)));
		}
		return;
	}
	setBusyWriting(false);
	writeStopped();
}

void Stm32Uart::stopRead()
{
	SEMF_INFO("stop read");
	HAL_StatusTypeDef state = HAL_UART_AbortReceive(m_hwHandle);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopRead_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopRead_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopRead_HalTimeout)));
		}
		return;
	}
	setBusyReading(false);
	readStopped();
}

void Stm32Uart::setFormat(uint8_t bits, Parity par, StopBits stop, FlowControl flow)
{
	SEMF_INFO("set format, bits %u, parity %u, stop %u, flow %u", bits, static_cast<uint32_t>(par), static_cast<uint32_t>(stop), static_cast<uint32_t>(flow));

	halLock();
	if (par == Parity::NoParity)
	{
		switch (bits)
		{
#if !defined(STM32F4) && !defined(STM32F1) && !defined(STM32F0)
			case 7:
				m_hwHandle->Init.WordLength = UART_WORDLENGTH_7B;
				break;
#endif
			case 8:
				m_hwHandle->Init.WordLength = UART_WORDLENGTH_8B;
				break;
			case 9:
				m_hwHandle->Init.WordLength = UART_WORDLENGTH_9B;
				break;
			default:
				SEMF_ERROR("word length invalid %u", bits);
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetFormat_WordLengthInvalid)));
				break;
		}
	}
	else
	{
		switch (bits)
		{
#if !defined(STM32F4) && !defined(STM32F1) && !defined(STM32F0)
			case 6:
				m_hwHandle->Init.WordLength = UART_WORDLENGTH_7B;
				break;
#endif
			case 7:
				m_hwHandle->Init.WordLength = UART_WORDLENGTH_8B;
				break;
			case 8:
				m_hwHandle->Init.WordLength = UART_WORDLENGTH_9B;
				break;
			default:
				SEMF_ERROR("word length invalid %u", bits);
				onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetFormat_WordLengthInvalid)));
				break;
		}
	}

	switch (par)
	{
		case Parity::NoParity:
			m_hwHandle->Init.Parity = UART_PARITY_NONE;
			break;
		case Parity::OddParity:
			m_hwHandle->Init.Parity = UART_PARITY_ODD;
			break;
		case Parity::EvenParity:
			m_hwHandle->Init.Parity = UART_PARITY_EVEN;
			break;
		default:
			SEMF_ERROR("parity invalid %u", static_cast<uint32_t>(par));
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetFormat_ParityInvalid)));
			return;
	}

	switch (stop)
	{
		case StopBits::Stopbits_1:
			m_hwHandle->Init.StopBits = UART_STOPBITS_1;
			break;
		case StopBits::Stopbits_2:
			m_hwHandle->Init.StopBits = UART_STOPBITS_2;
			break;
#if !defined(STM32F4) && !defined(STM32F1)
		case StopBits::Stopbits_0_5:
			m_hwHandle->Init.StopBits = UART_STOPBITS_0_5;
			break;
		case StopBits::Stopbits_1_5:
			m_hwHandle->Init.StopBits = UART_STOPBITS_1_5;
			break;
#endif
		default:
			SEMF_ERROR("stop bits invalid %u", static_cast<uint32_t>(stop));
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetFormat_StopBitsInvalid)));
			return;
	}

	switch (flow)
	{
		case FlowControl::NoFlowControl:
			m_hwHandle->Init.HwFlowCtl = UART_HWCONTROL_NONE;
			break;
		case FlowControl::RtsFlowControl:
			m_hwHandle->Init.HwFlowCtl = UART_HWCONTROL_RTS;
			break;
		case FlowControl::CtsFlowControl:
			m_hwHandle->Init.HwFlowCtl = UART_HWCONTROL_CTS;
			break;
		case FlowControl::RtsAndCtsFlowControl:
			m_hwHandle->Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
			break;
		default:
			SEMF_ERROR("flow invalid %u", static_cast<uint32_t>(flow));
			onError(Error(kSemfClassId, static_cast<uint32_t>(ErrorCode::SetFormat_FlowInvalid)));
			return;
	}
	__HAL_UNLOCK(m_hwHandle);
}

void Stm32Uart::setWireMode(WireMode mode)
{
	SEMF_INFO("set WireMode %u", mode);
	m_wireMode = mode;

	HAL_StatusTypeDef state = HAL_OK;
	HAL_UART_DeInit(m_hwHandle);

	switch (m_wireMode)
	{
		case WireMode::Duplex:
			state = HAL_UART_Init(m_hwHandle);
			break;
		case WireMode::Simplex:
			state = HAL_HalfDuplex_Init(m_hwHandle);
			break;
		default:
			SEMF_ERROR("WireMode is invalid %u", static_cast<uint32_t>(m_wireMode));
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_WireModeInvalid)));
			return;
	}

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

void Stm32Uart::setDirection(Direction direction)
{
	SEMF_INFO("set direction %u", static_cast<uint32_t>(direction));

	m_direction = direction;
	switch (m_direction)
	{
		case Direction::RxOnly:
			m_hwHandle->Init.Mode = UART_MODE_RX;
			break;
		case Direction::TxOnly:
			m_hwHandle->Init.Mode = UART_MODE_TX;
			break;
		case Direction::RxTx:
			m_hwHandle->Init.Mode = UART_MODE_TX_RX;
			break;
		default:
			SEMF_ERROR("direction is invalid %u", static_cast<uint32_t>(m_direction));
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetFormat_DirectionInvalid)));
			return;
	}
}

void Stm32Uart::setBaud(uint32_t baud)
{
	SEMF_INFO("set baud %u", baud);
	halLock();
	m_hwHandle->Init.BaudRate = baud;
	__HAL_UNLOCK(m_hwHandle);
}

uint32_t Stm32Uart::baud()
{
	return m_hwHandle->Init.BaudRate;
}

LinkedQueue<Stm32Uart>* Stm32Uart::queue()
{
	static LinkedQueue<Stm32Uart> queue;
	return &queue;
}

void Stm32Uart::systemIsrRead(UART_HandleTypeDef& uart)
{
	for (Stm32Uart& i : *(queue()))
		i.isrRead(uart);
}

void Stm32Uart::systemIsrWritten(UART_HandleTypeDef& uart)
{
	for (Stm32Uart& i : *(queue()))
		i.isrWritten(uart);
}

void Stm32Uart::isrRead(UART_HandleTypeDef& uart)
{
	if (m_hwHandle == &uart)
	{
		onDataAvailable();
	}
}

void Stm32Uart::isrWritten(UART_HandleTypeDef& uart)
{
	if (m_hwHandle == &uart)
	{
		onDataWritten();
	}
}

void Stm32Uart::writeHardware(const uint8_t data[], size_t dataSize)
{
	__HAL_UNLOCK(m_hwHandle);
	HAL_StatusTypeDef state = HAL_UART_Transmit_IT(m_hwHandle, const_cast<uint8_t*>(data), static_cast<uint16_t>(dataSize));
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

void Stm32Uart::readHardware(uint8_t buffer[], size_t bufferSize)
{
	__HAL_UNLOCK(m_hwHandle);
	HAL_StatusTypeDef state = HAL_UART_Receive_IT(m_hwHandle, buffer, static_cast<uint16_t>(bufferSize));
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReadHardware_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReadHardware_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReadHardware_HalTimeout)));
		}
		return;
	}
}

HAL_StatusTypeDef Stm32Uart::halLock()
{
	__HAL_LOCK(m_hwHandle);
	return HAL_OK;
}
} /* namespace semf */
#endif
