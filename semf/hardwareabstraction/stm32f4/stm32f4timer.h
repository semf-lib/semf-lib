/**
 * @file stm32f4timer.h
 * @date 31.01.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4TIMER_H_
#define SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4TIMER_H_

#include <semf/hardwareabstraction/stm32/stm32timer.h>

#if defined(STM32F4) && defined(HAL_TIM_MODULE_ENABLED)
namespace semf
{
/**
 * @brief Specialization of \c Stm32Timer for STM32F4.
 */
class Stm32F4Timer: public Stm32Timer
{
public:
	using Stm32Timer::Stm32Timer;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4TIMER_H_ */
