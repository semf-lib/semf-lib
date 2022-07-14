/**
 * @file	stm32f4outputcompare.h
 *
 * @date	12.03.2020
 * @author	aa
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <HardwareAbstraction/Stm32F4/stm32f4outputcompare.h>

#if defined(STM32F4) && defined(HAL_TIM_MODULE_ENABLED)

namespace semf
{
Stm32F4OutputCompare::Stm32F4OutputCompare(Mode mode, TIM_HandleTypeDef& hwHandle, uint32_t channel,
uint32_t timerFrequency /*khz*/)
: Stm32OutputCompare(mode, hwHandle, channel, timerFrequency)
{
	m_sConfigOC.OCMode = mode;
	m_sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	m_sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	m_sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	m_sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
}

TIM_OC_InitTypeDef* Stm32F4OutputCompare::configurationStructure()
{
	return &m_sConfigOC;
}

} /* namespace semf */

#endif
