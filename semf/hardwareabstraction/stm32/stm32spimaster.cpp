/**
 * @file stm32spimaster.cpp
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/hardwareabstraction/stm32/stm32spimaster.h>
#include <semf/utils/core/debug.h>
#include <algorithm>
#include <array>

#if defined(STM32) && defined(HAL_SPI_MODULE_ENABLED)
namespace semf
{
Stm32SpiMaster::Stm32SpiMaster(SPI_HandleTypeDef& hwHandle, uint32_t spiClockFrequencyHz)
: m_hwHandle(&hwHandle),
  m_spiClockFrequencyHz(spiClockFrequencyHz)
{
	queue()->push(*this);
	aborted.connect(m_abortedSlot);
}

Stm32SpiMaster::Stm32SpiMaster(SPI_HandleTypeDef& hwHandle, Gpio& chipSelectPin, uint32_t spiClockFrequencyHz)
: SpiMasterHardware(chipSelectPin),
  m_hwHandle(&hwHandle),
  m_spiClockFrequencyHz(spiClockFrequencyHz)
{
	queue()->push(*this);
	aborted.connect(m_abortedSlot);
}

void Stm32SpiMaster::init()
{
	SEMF_INFO("init");
	HAL_StatusTypeDef state = HAL_SPI_Init(m_hwHandle);
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

void Stm32SpiMaster::deinit()
{
	SEMF_INFO("deinit");
	HAL_StatusTypeDef state = HAL_SPI_DeInit(m_hwHandle);
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
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_HalError)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_HalError)));
		}
		return;
	}
}

void Stm32SpiMaster::setFrequency(uint32_t hz)
{
	if (isBusyReading() || isBusyWriting())
	{
		SEMF_ERROR("is busy");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetFrequency_IsBusy)));
	}

	if (m_spiClockFrequencyHz == 0)
	{
		SEMF_ERROR("Hz is zero");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetFrequency_SpiClockIsZero)));
	}

	deinit();
	std::array<uint32_t, 8> frequencies;

	uint8_t prescaler = 1;

	std::generate(frequencies.begin(), frequencies.end(),
				  [this, prescaler]() mutable
				  {
					  uint32_t freq = m_spiClockFrequencyHz / (prescaler + 1);
					  prescaler = static_cast<uint8_t>(prescaler | (prescaler << 1));
					  return freq;
				  });

	auto upperIt = std::upper_bound(frequencies.rbegin(), frequencies.rend(), hz);
	auto lowerIt = std::lower_bound(frequencies.rbegin(), frequencies.rend(), hz);

	if (*lowerIt != hz)
		lowerIt--;

	uint32_t upper = upperIt.base() == frequencies.end() ? frequencies.back() : *upperIt;
	uint32_t lower = lowerIt == frequencies.rend() ? frequencies.front() : *lowerIt;

	uint32_t best = upper - hz < hz - lower ? upper : lower;

	SEMF_INFO("closest possible frequency is %d Hz", best);

	auto bestIt = std::find(frequencies.begin(), frequencies.end(), best);

	if (bestIt - frequencies.begin() == 0)
		m_hwHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	else if (bestIt - frequencies.begin() == 1)
		m_hwHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	else if (bestIt - frequencies.begin() == 2)
		m_hwHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	else if (bestIt - frequencies.begin() == 3)
		m_hwHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	else if (bestIt - frequencies.begin() == 4)
		m_hwHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	else if (bestIt - frequencies.begin() == 5)
		m_hwHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	else if (bestIt - frequencies.begin() == 6)
		m_hwHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	else if (bestIt - frequencies.begin() == 7)
		m_hwHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;

	init();
}

LinkedQueue<Stm32SpiMaster>* Stm32SpiMaster::queue()
{
	static LinkedQueue<Stm32SpiMaster> queue;
	return &queue;
}

void Stm32SpiMaster::systemIsrRead(SPI_HandleTypeDef& spi)
{
	for (Stm32SpiMaster& i : *(queue()))
		i.isrRead(spi);
}

void Stm32SpiMaster::systemIsrWritten(SPI_HandleTypeDef& spi)
{
	for (Stm32SpiMaster& i : *(queue()))
		i.isrWritten(spi);
}

void Stm32SpiMaster::systemIsrWrittenAndRead(SPI_HandleTypeDef& spi)
{
	for (Stm32SpiMaster& i : *(queue()))
		i.isrWrittenAndRead(spi);
}

void Stm32SpiMaster::systemIsrError(SPI_HandleTypeDef& spi)
{
	for (Stm32SpiMaster& i : *(queue()))
		i.isrError(spi);
}

void Stm32SpiMaster::systemIsrAbort(SPI_HandleTypeDef& spi)
{
	for (Stm32SpiMaster& i : *(queue()))
		i.isrAborted(spi);
}

void Stm32SpiMaster::isrRead(SPI_HandleTypeDef& spi)
{
	if (&spi != m_hwHandle)
		return;

	m_dataIndex++;

	// Problem in HAL_SPI_Receive, so we always do one byte after the other
	if (m_dataIndex < m_dataSize)
	{
		readNext();
	}
	else
	{
		if (frame() == CommunicationHardware::Frame::Last || frame() == CommunicationHardware::Frame::FirstAndLast)
			disableChipSelect();
		onDataAvailable();
	}
}

void Stm32SpiMaster::isrWritten(SPI_HandleTypeDef& spi)
{
	if (&spi != m_hwHandle)
		return;

	if (frame() == CommunicationHardware::Frame::Last || frame() == CommunicationHardware::Frame::FirstAndLast)
		disableChipSelect();

	onDataWritten();
}

void Stm32SpiMaster::isrWrittenAndRead(SPI_HandleTypeDef& spi)
{
	if (&spi != m_hwHandle)
		return;

	m_dataIndex++;

	// Problem in HAL_SPI_Transmit, so we always do one byte after the other
	if (m_dataIndex < m_dataSize)
	{
		writeReadNext();
	}
	else  // Finished
	{
		if (frame() == CommunicationHardware::Frame::Last || frame() == CommunicationHardware::Frame::FirstAndLast)
			disableChipSelect();

		onDataAvailable();
	}
}

void Stm32SpiMaster::isrError(SPI_HandleTypeDef& spi)
{
	if (&spi != m_hwHandle)
		return;

	SEMF_INFO("isr error");
	onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::IsrError_Isr)));
}

void Stm32SpiMaster::isrAborted(SPI_HandleTypeDef& spi)
{
	if (&spi != m_hwHandle)
		return;

	SEMF_INFO("isr abort");
	setBusy(false);
	aborted();
}

void Stm32SpiMaster::writeHardware(const uint8_t data[], size_t dataSize)
{
	SEMF_INFO("data %p, size %u", data, dataSize);
	__HAL_UNLOCK(m_hwHandle);
	HAL_StatusTypeDef state = HAL_SPI_Transmit_IT(m_hwHandle, const_cast<uint8_t*>(data), static_cast<uint16_t>(dataSize));
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Write_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Write_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Write_HalTimeout)));
		}
		return;
	}
}

void Stm32SpiMaster::readHardware(uint8_t buffer[], size_t bufferSize)
{
	SEMF_INFO("data %p, size %u", buffer, bufferSize);
	m_readData = const_cast<uint8_t*>(buffer);
	m_dataSize = bufferSize;
	m_dataIndex = 0;
	readNext();
}

void Stm32SpiMaster::writeReadHardware(const uint8_t writeData[], uint8_t readBuffer[], size_t size)
{
	SEMF_INFO("write data %p, read data %p, size %u", writeData, readBuffer, size);
	m_writeData = writeData;
	m_readData = readBuffer;
	m_dataSize = size;
	m_dataIndex = 0;
	writeReadNext();
}

void Stm32SpiMaster::setFormatHardware(uint8_t bits, Stm32SpiMaster::TransmissionMode transmission, Stm32SpiMaster::WireMode wire)
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

void Stm32SpiMaster::stopWriteHardware()
{
	m_abortedSlot.setObject(writeStopped);
	auto state = HAL_SPI_Abort_IT(m_hwHandle);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopWriteHardware_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopWriteHardware_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopWriteHardware_HalTimeout)));
		}
		return;
	}
}

void Stm32SpiMaster::stopReadHardware()
{
	m_abortedSlot.setObject(readStopped);
	auto state = HAL_SPI_Abort_IT(m_hwHandle);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopReadHardware_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopReadHardware_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopReadHardware_HalTimeout)));
		}
		return;
	}
}

void Stm32SpiMaster::readNext()
{
	SEMF_INFO("read next");
	__HAL_UNLOCK(m_hwHandle);
	HAL_StatusTypeDef state = HAL_SPI_Receive_IT(m_hwHandle, &m_readData[m_dataIndex], 1);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReadNext_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReadNext_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReadNext_HalTimeout)));
		}
		return;
	}
}

void Stm32SpiMaster::writeReadNext()
{
	SEMF_INFO("write / read next");
	__HAL_UNLOCK(m_hwHandle);
	HAL_StatusTypeDef state = HAL_SPI_TransmitReceive_IT(m_hwHandle, const_cast<uint8_t*>(&m_writeData[m_dataIndex]), &m_readData[m_dataIndex], 1);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteReadNext_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteReadNext_HalError)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteReadNext_HalError)));
		}
		return;
	}
}
} /* namespace semf */
#endif
