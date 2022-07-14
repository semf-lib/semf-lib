/**
 * @file stm32analogin.cpp
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <HardwareAbstraction/Stm32/stm32analogin.h>
#include <Core/debug.h>

#if defined(STM32) && defined(HAL_ADC_MODULE_ENABLED)

namespace semf
{
Stm32AnalogIn::Stm32AnalogIn(ADC_HandleTypeDef& hwHandle)
	:m_hwHandle(&hwHandle)
{
	queue()->push(*this);
}

void Stm32AnalogIn::start()
{
	SEMF_INFO("start");
	HAL_StatusTypeDef state = HAL_ADC_Start_IT(m_hwHandle);
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

uint32_t Stm32AnalogIn::value()
{
	uint32_t val = HAL_ADC_GetValue(m_hwHandle);
	SEMF_INFO("value %u", val);
	return val;
}

void Stm32AnalogIn::stop()
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

LinkedQueue<Stm32AnalogIn>* Stm32AnalogIn::queue()
{
	static LinkedQueue<Stm32AnalogIn> queue;
	return &queue;
}

void Stm32AnalogIn::systemIsr(ADC_HandleTypeDef& adc)
{
	for (Stm32AnalogIn& i : *(queue()))
		i.isr(adc);
}

void Stm32AnalogIn::systemIsrError(ADC_HandleTypeDef& adc)
{
	for (Stm32AnalogIn& i : *(queue()))
		i.isrError(adc);
}

void Stm32AnalogIn::isr(ADC_HandleTypeDef& adc)
{
	if (&adc == m_hwHandle)
	{
		SEMF_INFO("dataAvailable");
		dataAvailable();
	}
}

void Stm32AnalogIn::isrError(ADC_HandleTypeDef& adc)
{
	if (&adc == m_hwHandle)
	{
		SEMF_ERROR("error");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::IsrError_Isr)));
	}
}

} /* namespace semf */

#endif
