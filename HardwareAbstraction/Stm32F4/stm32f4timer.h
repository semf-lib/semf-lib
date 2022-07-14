/**
 * @file stm32f4timer.h
 *
 * @date 31.01.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32F4_STM32F4TIMER_H_
#define HARDWAREABSTRACTION_STM32F4_STM32F4TIMER_H_

#include <HardwareAbstraction/Stm32/stm32timer.h>

#if defined(STM32F4) && defined(HAL_TIM_MODULE_ENABLED)
namespace semf
{
class Stm32F4Timer: public Stm32Timer
{
public:
	using Stm32Timer::Stm32Timer;
};

} /* namespace semf */

#endif

#endif /* HARDWAREABSTRACTION_STM32F4_STM32F4TIMER_H_ */
