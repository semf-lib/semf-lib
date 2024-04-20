/**
 * @file stm32analogindma.cpp
 * @date 26.04.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/hardwareabstraction/stm32/stm32analogindma.h>
#include <semf/utils/core/debug.h>

#if defined(STM32) && defined(HAL_ADC_MODULE_ENABLED)
namespace semf
{
LinkedQueue<Stm32AnalogInDma> Stm32AnalogInDma::m_queue;
Stm32AnalogInDma::Stm32AnalogInDma(ADC_HandleTypeDef& hwHandle)
: m_hwHandle(&hwHandle)
{
	queue()->push(*this);
}

void Stm32AnalogInDma::start(uint8_t buffer[], size_t bufferSize)
{
	SEMF_INFO("start data: %p, size: %u", buffer, bufferSize);

#if !defined(STM32F4) && !defined STM32F7
	if (m_hwHandle->DMA_Handle && m_hwHandle->DMA_Handle->Init.MemDataAlignment)
		bufferSize /= (m_hwHandle->DMA_Handle->Init.MemDataAlignment >> (DMA_CCR_MSIZE_Pos - 1));
#endif

	HAL_StatusTypeDef state = HAL_ADC_Start_DMA(m_hwHandle, reinterpret_cast<uint32_t*>(buffer), static_cast<uint32_t>(bufferSize));
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

void Stm32AnalogInDma::stop()
{
	SEMF_INFO("stop");
	HAL_StatusTypeDef state = HAL_ADC_Stop_DMA(m_hwHandle);
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

LinkedQueue<Stm32AnalogInDma>* Stm32AnalogInDma::queue()
{
	static LinkedQueue<Stm32AnalogInDma> queue;
	return &queue;
}

void Stm32AnalogInDma::systemIsr(ADC_HandleTypeDef& adc)
{
	for (Stm32AnalogInDma& i : *(queue()))
		i.isr(adc);
}

void Stm32AnalogInDma::systemIsrError(ADC_HandleTypeDef& adc)
{
	for (Stm32AnalogInDma& i : *(queue()))
		i.isrError(adc);
}

void Stm32AnalogInDma::isr(ADC_HandleTypeDef& adc)
{
	if (&adc == m_hwHandle)
	{
		SEMF_INFO("dataAvailable");
		dataAvailable();
	}
}

void Stm32AnalogInDma::isrError(ADC_HandleTypeDef& adc)
{
	if (&adc == m_hwHandle)
	{
		SEMF_INFO("error");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::IsrError_Isr)));
	}
}
} /* namespace semf */
#endif
