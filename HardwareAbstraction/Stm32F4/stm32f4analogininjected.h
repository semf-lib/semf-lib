/**
 * @file stm32f4analogininjected.h
 *
 * @date 09.10.2020
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGININJECTED_H_
#define HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGININJECTED_H_

#include <HardwareAbstraction/Stm32/stm32analogininjected.h>

#if defined(STM32F4) && defined(HAL_ADC_MODULE_ENABLED)
namespace semf
{
class Stm32F4AnalogInInjected : public Stm32AnalogInInjected
{
public:
	using Stm32AnalogInInjected::Stm32AnalogInInjected;
};

} /* namespace semf */

#endif

#endif /* HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGININJECTED_H_ */
