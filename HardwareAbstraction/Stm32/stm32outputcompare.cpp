/**
 * @file	stm32outputcompare.cpp
 *
 * @date	12.03.2020
 * @author	aa
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <HardwareAbstraction/Stm32/stm32outputcompare.h>
#include <Core/debug.h>

#if defined(STM32) && defined(HAL_TIM_MODULE_ENABLED)

extern "C"
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef* htim)
{
	semf::Stm32OutputCompare::moduleIsr(htim);
}

namespace semf
{
LinkedQueue<Stm32OutputCompare> Stm32OutputCompare::m_queue;

Stm32OutputCompare::Stm32OutputCompare(Mode mode, TIM_HandleTypeDef& hwHandle, uint32_t channel,
		uint32_t timerFrequency)
	: m_mode(mode),  m_hwHandle(&hwHandle),  m_channel(channel),
	  m_frequencyBeforePrescaling(timerFrequency)
{
	queue()->push(*this);
}

LinkedQueue<Stm32OutputCompare>* Stm32OutputCompare::queue()
{
	return &m_queue;
}

void Stm32OutputCompare::moduleIsr(TIM_HandleTypeDef* hwHandle)
{
	for (Stm32OutputCompare& i : *(queue()))
	{
		if (i.handle() == hwHandle)
			i.isr();
	}
}

void Stm32OutputCompare::setMode(Mode mode)
{
	SEMF_INFO("set mode to %u", static_cast<uint32_t>(mode));
	m_mode = mode;
	configurationStructure()->OCMode = m_mode;
	initHardware();
	if (isEnabled())
	{
		HAL_StatusTypeDef state = HAL_TIM_OC_Start_IT(m_hwHandle, m_channel);
		if (state != HAL_OK)
		{
			if (state == HAL_ERROR)
			{
				SEMF_ERROR("hal error");
				error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetMode_HalError)));
			}
			else if (state == HAL_BUSY)
			{
				SEMF_ERROR("hal busy");
				error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetMode_HalBusy)));
			}
			else if (state == HAL_TIMEOUT)
			{
				SEMF_ERROR("hal timeout");
				error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetMode_HalTimeout)));
			}
			return;
		}
	}
}

void Stm32OutputCompare::initHardware()
{
	SEMF_INFO("init hardware");
	HAL_StatusTypeDef state = HAL_TIM_OC_ConfigChannel(m_hwHandle, configurationStructure(), m_channel);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::InitHardware_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::InitHardware_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::InitHardware_HalTimeout)));
		}
		return;
	}
}

void Stm32OutputCompare::startHardware()
{
	HAL_StatusTypeDef state = HAL_TIM_OC_Start_IT(m_hwHandle, m_channel);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StartHardware_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StartHardware_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StartHardware_HalTimeout)));
		}
		return;
	}
}

void Stm32OutputCompare::stopHardware()
{
	HAL_StatusTypeDef state = HAL_TIM_OC_Stop_IT(m_hwHandle, m_channel);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopHardware_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopHardware_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopHardware_HalTimeout)));
		}
		return;
	}
}

void Stm32OutputCompare::setTimerConfiguration()
{
	SEMF_INFO("set configuration");
	m_hwHandle->Init.Prescaler = 0;
	uint32_t value = m_frequencyBeforePrescaling*1000 / frequency();
	while (value > MaxArrValue)
	{
		m_hwHandle->Init.Prescaler += 1;
		value = m_frequencyBeforePrescaling*1000 / (m_hwHandle->Init.Prescaler + 1) / frequency();
	}
	m_hwHandle->Init.Period = value;
	HAL_StatusTypeDef state = HAL_TIM_Base_Init(m_hwHandle);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetTimerConfiguration_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetTimerConfiguration_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetTimerConfiguration_HalTimeout)));
		}
		return;
	}
}

uint32_t Stm32OutputCompare::timerInputFrequency() const
{
	return m_frequencyBeforePrescaling*1000;
}

void Stm32OutputCompare::isr()
{
	SEMF_INFO("timeout");
	timeout();
}

TIM_HandleTypeDef* Stm32OutputCompare::handle()
{
	return m_hwHandle;
}
}  /* namespace semf */

#endif
