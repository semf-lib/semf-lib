/**
 * @file stm32f4gpio.h
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32F4_STM32F4GPIO_H_
#define HARDWAREABSTRACTION_STM32F4_STM32F4GPIO_H_

#include <HardwareAbstraction/Stm32/stm32.h>

#include <HardwareAbstraction/Stm32/stm32gpio.h>

#if defined(STM32F4)
namespace semf
{
class Stm32F4Gpio : public Stm32Gpio
{
public:
	using Stm32Gpio::Stm32Gpio;
};

} /* namespace semf */

#endif

#endif /* HARDWAREABSTRACTION_STM32F4_STM32F4GPIO_H_ */
