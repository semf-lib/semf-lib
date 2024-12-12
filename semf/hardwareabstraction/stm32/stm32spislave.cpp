/**
 * @file stm32spislave.cpp
 * @date 27.01.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/hardwareabstraction/stm32/stm32spislave.h>
#include <semf/utils/core/debug.h>

#if defined(STM32) && defined(HAL_SPI_MODULE_ENABLED)
namespace semf
{
LinkedQueue<Stm32SpiSlave> Stm32SpiSlave::m_queue;

Stm32SpiSlave::Stm32SpiSlave(SPI_HandleTypeDef& hwHandle, bool useDma)
: m_hwHandle{&hwHandle},
  m_useDma{useDma}
{
	m_queue.push(*this);
	aborted.connect(m_abortedSlot);
}

void Stm32SpiSlave::init()
{
	auto state = HAL_SPI_Init(m_hwHandle);
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
	setBusy(false);
}
void Stm32SpiSlave::deinit()
{
	auto state = HAL_SPI_DeInit(m_hwHandle);
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
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Deinit_HalError)));
		}
		return;
	}
	setBusy(false);
}

void Stm32SpiSlave::systemIsrRead(SPI_HandleTypeDef& hwHandle)
{
	for (auto& handle : m_queue)
	{
		if (handle.m_hwHandle == &hwHandle)
			handle.onDataAvailable();
	}
}

void Stm32SpiSlave::systemIsrWritten(SPI_HandleTypeDef& hwHandle)
{
	for (auto& handle : m_queue)
	{
		if (handle.m_hwHandle == &hwHandle)
			handle.onDataWritten();
	}
}

void Stm32SpiSlave::systemIsrWrittenAndRead(SPI_HandleTypeDef& hwHandle)
{
	for (auto& handle : m_queue)
	{
		if (handle.m_hwHandle == &hwHandle)
			handle.onReadWrittenHardware();
	}
}

void Stm32SpiSlave::systemIsrError(SPI_HandleTypeDef& hwHandle)
{
	for (auto& handle : m_queue)
	{
		if (handle.m_hwHandle == &hwHandle)
			handle.onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SystemIsrError_Isr)));
	}
}

void Stm32SpiSlave::systemIsrAbort(SPI_HandleTypeDef& hwHandle)
{
	for (auto& handle : m_queue)
	{
		if (handle.m_hwHandle == &hwHandle)
			handle.onAbortedHardware();
	}
}

void Stm32SpiSlave::writeHardware(const uint8_t data[], size_t dataSize)
{
	__HAL_UNLOCK(m_hwHandle);
	HAL_StatusTypeDef state = HAL_OK;
	if (m_useDma)
	{
		state = HAL_SPI_Transmit_DMA(m_hwHandle, const_cast<uint8_t*>(data), static_cast<uint16_t>(dataSize));
	}
	else
	{
		state = HAL_SPI_Transmit_IT(m_hwHandle, const_cast<uint8_t*>(data), static_cast<uint16_t>(dataSize));
	}

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

void Stm32SpiSlave::readHardware(uint8_t buffer[], size_t bufferSize)
{
	__HAL_UNLOCK(m_hwHandle);
	HAL_StatusTypeDef state = HAL_OK;
	if (m_useDma)
	{
		state = HAL_SPI_Receive_DMA(m_hwHandle, const_cast<uint8_t*>(buffer), static_cast<uint16_t>(bufferSize));
	}
	else
	{
		state = HAL_SPI_Receive_IT(m_hwHandle, const_cast<uint8_t*>(buffer), static_cast<uint16_t>(bufferSize));
	}

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

void Stm32SpiSlave::writeReadHardware(const uint8_t writeData[], uint8_t readBuffer[], size_t size)
{
	m_writeData = const_cast<uint8_t*>(writeData);
	m_readBuffer = readBuffer;
	m_writeDataSize = size;

	__HAL_UNLOCK(m_hwHandle);
	HAL_StatusTypeDef state = HAL_OK;
	if (m_useDma)
	{
		state = HAL_SPI_TransmitReceive_DMA(m_hwHandle, const_cast<uint8_t*>(writeData), readBuffer, static_cast<uint16_t>(size));
	}
	else
	{
		state = HAL_SPI_TransmitReceive_IT(m_hwHandle, const_cast<uint8_t*>(writeData), readBuffer, static_cast<uint16_t>(size));
	}

	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteReadHardware_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteReadHardware_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteReadHardware_HalTimeout)));
		}
		return;
	}
}

void Stm32SpiSlave::stopReadHardware()
{
	m_abortedSlot.setObject(readStopped);
	__HAL_UNLOCK(m_hwHandle);
	auto state = HAL_SPI_Abort(m_hwHandle);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopReadHardware_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopReadHardware_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopReadHardware_HalTimeout)));
		}
		return;
	}
}

void Stm32SpiSlave::stopWriteHardware()
{
	m_abortedSlot.setObject(writeStopped);
	__HAL_UNLOCK(m_hwHandle);
	auto state = HAL_SPI_Abort(m_hwHandle);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopWriteHardware_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopWriteHardware_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopWriteHardware_HalTimeout)));
		}
		return;
	}
}

void Stm32SpiSlave::onReadWritten()
{
	SEMF_INFO("read written");
	setBusy(false);
	dataAvailable();
}

void Stm32SpiSlave::onReadWrittenHardware()
{
	m_writeData++;
	m_readBuffer++;
	m_writeDataSize--;
	if (m_writeDataSize != 0)
	{
		writeReadHardware(m_writeData, m_readBuffer, m_writeDataSize);
	}
	else
	{
		onReadWritten();
	}
}

void Stm32SpiSlave::onAbortedHardware()
{
	setBusy(false);
	aborted();
}

void Stm32SpiSlave::setFormatHardware(uint8_t bits, Stm32SpiSlave::TransmissionMode transmission, Stm32SpiSlave::WireMode wire)
{
	if (bits == 8)
		m_hwHandle->Init.DataSize = SPI_DATASIZE_8BIT;
	else if (bits == 16)
		m_hwHandle->Init.DataSize = SPI_DATASIZE_16BIT;
	else
	{
		SEMF_ERROR("%d bits not supported");
		onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetFormatHardware_BitsNotSupported)));
		return;
	}

	switch (transmission)
	{
		case TransmissionMode::Mode0:
			m_hwHandle->Init.CLKPolarity = SPI_POLARITY_LOW;
			m_hwHandle->Init.CLKPhase = SPI_PHASE_1EDGE;
			break;
		case TransmissionMode::Mode1:
			m_hwHandle->Init.CLKPolarity = SPI_POLARITY_LOW;
			m_hwHandle->Init.CLKPhase = SPI_PHASE_2EDGE;
			break;
		case TransmissionMode::Mode2:
			m_hwHandle->Init.CLKPolarity = SPI_POLARITY_HIGH;
			m_hwHandle->Init.CLKPhase = SPI_PHASE_1EDGE;
			break;
		case TransmissionMode::Mode3:
			m_hwHandle->Init.CLKPolarity = SPI_POLARITY_HIGH;
			m_hwHandle->Init.CLKPhase = SPI_PHASE_2EDGE;
			break;
	}

	switch (wire)
	{
		case WireMode::WriteOnlyWires:
		case WireMode::FullDuplexWires:
			m_hwHandle->Init.Direction = SPI_DIRECTION_2LINES;
			break;
		case WireMode::HalfDuplexWires:
			m_hwHandle->Init.Direction = SPI_DIRECTION_1LINE;
			break;
		case WireMode::ReadOnlyWires:
			m_hwHandle->Init.Direction = SPI_DIRECTION_2LINES_RXONLY;
			break;
	}
}
}  // namespace semf
#endif
