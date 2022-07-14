/**
 * @file stm32analogoutdma.cpp
 *
 * @date 24.02.2020
 * @author sga
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */
#include <HardwareAbstraction/Stm32/stm32analogoutdma.h>
#include <Core/debug.h>

#if defined(STM32) && defined(HAL_DAC_MODULE_ENABLED)
namespace semf
{
Stm32AnalogOutDma::Stm32AnalogOutDma(DAC_HandleTypeDef& hwHandle, uint32_t channel, Alignment alignment)
	: m_hwHandle(hwHandle), m_channel(channel), m_alignment(alignment)
{
	queue()->push(*this);
}

void Stm32AnalogOutDma::start() const
{
	SEMF_INFO("start");
	if (m_data == nullptr || m_size == 0)
	{
		SEMF_ERROR("data is nullptr or size is zero");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Start_DataInvalid)));
		return;
	}
	HAL_StatusTypeDef state = HAL_DAC_Start_DMA(&m_hwHandle, m_channel,
			reinterpret_cast<uint32_t*>(const_cast<uint8_t*>(m_data)), m_size/4, alignment(m_alignment));
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("start hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Start_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("start hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Start_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("start hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Start_HalTimeout)));
		}
		return;
	}
}

void Stm32AnalogOutDma::stop() const
{
	SEMF_INFO("stop");
	HAL_StatusTypeDef state = HAL_DAC_Stop_DMA(&m_hwHandle, m_channel);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("stop hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Stop_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("stop hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Stop_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("stop hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Stop_HalTimeout)));
		}
		return;
	}
}

void Stm32AnalogOutDma::setData(const uint8_t data[], size_t dataSize)
{
	SEMF_INFO("set data on channel %u", m_channel);
	m_data = data;
	m_size = dataSize;
}

LinkedQueue<Stm32AnalogOutDma>* Stm32AnalogOutDma::queue()
{
	static LinkedQueue<Stm32AnalogOutDma> queue;
	return &queue;
}

void Stm32AnalogOutDma::systemIsrError(DAC_HandleTypeDef& dac)
{
	for (Stm32AnalogOutDma& i : *(queue()))
		i.isrError(dac);
}

void Stm32AnalogOutDma::systemIsr(DAC_HandleTypeDef& dac)
{
	for (Stm32AnalogOutDma& i : *(queue()))
		i.isr(dac);
}

void Stm32AnalogOutDma::isrError(DAC_HandleTypeDef& dac)
{
	if (&dac == &m_hwHandle)
	{
		SEMF_ERROR("error");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::IsrError_Isr)));
	}
}

void Stm32AnalogOutDma::isr(DAC_HandleTypeDef& dac)
{
	if (&dac == &m_hwHandle)
	{
		SEMF_INFO("dataAvailable");
		dataWritten();
	}
}

uint32_t Stm32AnalogOutDma::alignment(Alignment setting) const
{
	switch(setting)
	{
	case Alignment::Align_8bitRight:
		return DAC_ALIGN_8B_R;
		break;
	case Alignment::Align_12bitLeft:
		return DAC_ALIGN_12B_L;
		break;
	case Alignment::Align_12bitRight:
		return DAC_ALIGN_12B_R;
		break;
	default:
		SEMF_ERROR("invalid alignment setting %u", static_cast<uint8_t>(setting));
		return 0;
	}
}

} /* namespace semf */
#endif /* STM32 & HAL_DAC_MODULE_ENABLED */
