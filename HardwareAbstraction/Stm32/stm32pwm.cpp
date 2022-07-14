/**
 * @file stm32pwm.cpp
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <HardwareAbstraction/Stm32/stm32pwm.h>
#include <Core/debug.h>

#if defined(STM32) && defined(HAL_TIM_MODULE_ENABLED)
namespace semf
{
Stm32Pwm::Stm32Pwm(TIM_HandleTypeDef& hwHandle, uint32_t channel, bool fastMode, Mode mode)
	:m_hwHandle(&hwHandle),
	 m_channel(channel),
	 m_fastModeEnabled(fastMode),
	 m_mode(mode)
{

}

void Stm32Pwm::setMaxValue(unsigned int maxValue)
{
	SEMF_INFO("set max value %u", maxValue);
	m_hwHandle->Instance->ARR = maxValue;
}

unsigned int Stm32Pwm::maxValue() const
{
	return m_hwHandle->Instance->ARR;
}

void Stm32Pwm::set(unsigned int value, bool inverted)
{
	SEMF_INFO("set: value %u, inverted: %d", value, inverted);
	// Secure having flickers by updating the hardware register without changing the value.
	if (value == m_pwmValue && value != 0)
	{
		SEMF_INFO("value is m_pwmValue and is not zero, return");
		return;
	}
	m_pwmValue = value;

	TIM_OC_InitTypeDef sConfigOC;
	if (m_mode == Mode::Mode1)
	{
		sConfigOC.OCMode = TIM_OCMODE_PWM1;
	}
	else
	{
		sConfigOC.OCMode = TIM_OCMODE_PWM2;
	}
	sConfigOC.Pulse = value;
	if (inverted)
	{
		sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
	}
	else
	{
		sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	}
	if (m_fastModeEnabled)
	{
		sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
	}
	else
	{
		sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	}
#if !defined(STM32L0)
	sConfigOC.OCIdleState = 0;
	sConfigOC.OCNIdleState = 0;
#endif
	HAL_StatusTypeDef state = HAL_TIM_PWM_ConfigChannel(m_hwHandle, &sConfigOC, m_channel);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Set_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Set_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Set_HalTimeout)));
		}
		return;
	}

	if (m_enabled)
	{
		start();
	}
}

void Stm32Pwm::start()
{
	m_enabled = true;
	SEMF_INFO("start");
	HAL_StatusTypeDef state = HAL_TIM_PWM_Start(m_hwHandle, m_channel);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Start_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Start_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Start_HalTimeout)));
		}
		return;
	}
}

unsigned int Stm32Pwm::value() const
{
	return m_pwmValue;
}

void Stm32Pwm::stop()
{
	m_enabled = false;
	SEMF_INFO("stop");
	HAL_StatusTypeDef state = HAL_TIM_PWM_Stop(m_hwHandle, m_channel);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Stop_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Stop_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Stop_HalTimeout)));
		}
		return;
	}
}

bool Stm32Pwm::isEnabled() const
{
	return m_enabled;
}

} /* namespace semf */

#endif
