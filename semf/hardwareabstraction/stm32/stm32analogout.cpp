/**
 * @file stm32analogout.cpp
 * @date 24.02.2020
 * @author sga
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */
#include <semf/hardwareabstraction/stm32/stm32analogout.h>
#include <semf/utils/core/debug.h>

#if defined(STM32) && defined(HAL_DAC_MODULE_ENABLED)
namespace semf
{
Stm32AnalogOut::Stm32AnalogOut(DAC_HandleTypeDef& hwHandle, uint32_t channel, Alignment alignment)
: m_hwHandle(hwHandle),
  m_channel(channel),
  m_alignment(alignment)
{
	queue()->push(*this);
}

void Stm32AnalogOut::start()
{
	SEMF_INFO("start");
	HAL_StatusTypeDef state = HAL_DAC_Start(&m_hwHandle, m_channel);
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

void Stm32AnalogOut::stop()
{
	SEMF_INFO("stop");
	HAL_StatusTypeDef state = HAL_DAC_Stop(&m_hwHandle, m_channel);
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

void Stm32AnalogOut::setValue(uint32_t value)
{
	SEMF_INFO("set value on channel %u to %u", m_channel, value);
	HAL_StatusTypeDef state = HAL_DAC_SetValue(&m_hwHandle, m_channel, alignment(m_alignment), value);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("set hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetValue_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("set hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetValue_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("set hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetValue_HalTimeout)));
		}
		return;
	}
}

LinkedQueue<Stm32AnalogOut>* Stm32AnalogOut::queue()
{
	static LinkedQueue<Stm32AnalogOut> queue;
	return &queue;
}

void Stm32AnalogOut::systemIsrError(DAC_HandleTypeDef& dac)
{
	for (Stm32AnalogOut& i : *(queue()))
		i.isrError(dac);
}

void Stm32AnalogOut::isrError(DAC_HandleTypeDef& dac)
{
	if (&dac == &m_hwHandle)
	{
		SEMF_ERROR("error");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::IsrError_Isr)));
	}
}

uint32_t Stm32AnalogOut::alignment(Alignment setting) const
{
	switch (setting)
	{
		case Alignment::Align8bitRight:
			return DAC_ALIGN_8B_R;
			break;
		case Alignment::Align12bitLeft:
			return DAC_ALIGN_12B_L;
			break;
		case Alignment::Align12bitRight:
			return DAC_ALIGN_12B_R;
			break;
		default:
			SEMF_ERROR("invalid alignment setting %u", static_cast<uint8_t>(setting));
			return 0;
	}
}
} /* namespace semf */
#endif /* STM32 & HAL_DAC_MODULE_ENABLED */
