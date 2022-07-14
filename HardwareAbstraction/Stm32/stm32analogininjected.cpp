/**
 * @file stm32analogininjected.cpp
 *
 * @date 07.10.2020
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <HardwareAbstraction/Stm32/stm32analogininjected.h>
#include <Core/debug.h>

#if defined(STM32) && defined(HAL_ADC_MODULE_ENABLED)
#if defined(STM32F1) || defined(STM32F3) || defined(STM32F4) || defined(STM32F7)

namespace semf
{
Stm32AnalogInInjected::Stm32AnalogInInjected(ADC_HandleTypeDef& hwHandle, uint32_t injectedRank)
	:m_hwHandle(&hwHandle),
	 m_injectedRank(injectedRank)
{
	queue()->push(*this);
}

void Stm32AnalogInInjected::start()
{
	SEMF_INFO("start");

	HAL_StatusTypeDef state = HAL_ADCEx_InjectedStart_IT(m_hwHandle);
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

uint32_t Stm32AnalogInInjected::value()
{
	uint32_t val = HAL_ADCEx_InjectedGetValue(m_hwHandle, m_injectedRank);
	SEMF_INFO("value %u", val);
	return val;
}

void Stm32AnalogInInjected::stop()
{
	SEMF_INFO("stop");
	HAL_StatusTypeDef state = HAL_ADC_Stop_IT(m_hwHandle);
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

LinkedQueue<Stm32AnalogInInjected>* Stm32AnalogInInjected::queue()
{
	static LinkedQueue<Stm32AnalogInInjected> queue;
	return &queue;
}

void Stm32AnalogInInjected::systemIsr(ADC_HandleTypeDef& adc)
{
	for (Stm32AnalogInInjected& i : *(queue()))
		i.isr(adc);
}

void Stm32AnalogInInjected::systemIsrError(ADC_HandleTypeDef& adc)
{
	for (Stm32AnalogInInjected& i : *(queue()))
		i.isrError(adc);
}

void Stm32AnalogInInjected::isr(ADC_HandleTypeDef& adc)
{
	if (&adc == m_hwHandle)
	{
		SEMF_INFO("dataAvailable");
		dataAvailable();
	}
}

void Stm32AnalogInInjected::isrError(ADC_HandleTypeDef& adc)
{
	if (&adc == m_hwHandle)
	{
		SEMF_INFO("error");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::IsrError_Isr)));
	}
}

} /* namespace semf */

#endif
#endif
