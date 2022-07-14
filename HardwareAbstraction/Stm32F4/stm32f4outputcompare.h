/**
 * @file	stm32f4outputcompare.h
 *
 * @date	12.03.2020
 * @author	aa
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32F4_STM32F4OUTPUTCOMPARE_H_
#define HARDWAREABSTRACTION_STM32F4_STM32F4OUTPUTCOMPARE_H_

#include <HardwareAbstraction/Stm32/stm32outputcompare.h>

#if defined(STM32F4) && defined(HAL_TIM_MODULE_ENABLED)
namespace semf
{
class Stm32F4OutputCompare: public Stm32OutputCompare
{
public:
	/**
	 * @brief Constructor for \c OutputCompare hardware module of \c Stm32F4 microcontroller.
	 * @param mode Working mode of hardware module @see Mode.
	 * @param hwHandle Hardware handler of the timer module.
	 * @param channel Output channel, if \c TIMING mode is selected, please pass channel zero of the hardware module.
	 * @param timerFrequency Base frequency of the used timer before precaling [KHz].
	 */
	Stm32F4OutputCompare(Mode mode, TIM_HandleTypeDef& hwHandle, uint32_t channel, uint32_t timerFrequency /*khz*/);
private:
	TIM_OC_InitTypeDef* configurationStructure();
	TIM_OC_InitTypeDef m_sConfigOC = { 0, 0, 0, 0, 0, 0, 0 };  /**< Stores the hardware module configuration.*/
};

}  /* namespace semf */
#endif

#endif /* HARDWAREABSTRACTION_STM32F4_STM32F4OUTPUTCOMPARE_H_ */
