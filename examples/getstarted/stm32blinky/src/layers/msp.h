/**
 * @file msp.h
 *
 * @date 09.07.2024
 * @author mm
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */
#ifndef EXAMPLES_GETSTARTED_STM32BLINKY_SRC_LAYERS_MSP_H_
#define EXAMPLES_GETSTARTED_STM32BLINKY_SRC_LAYERS_MSP_H_

#include <semf/hardwareabstraction/stm32f4/stm32f4gpio.h>
#include <semf/hardwareabstraction/stm32f4/stm32f4systick.h>

namespace msp
{
class Msp
{
public:
	Msp();
	explicit Msp(const Msp& other) = delete;
	~Msp() = default;

	semf::Stm32F4Gpio& gpioLd3();
	semf::Stm32F4Gpio& gpioB1();
	semf::Stm32F4Systick& systick1ms();

private:
	semf::Stm32F4Gpio m_gpioLd3;
	semf::Stm32F4Gpio m_gpioB1;
	semf::Stm32F4Systick& m_systick1ms;
};
}  // namespace msp
#endif  // EXAMPLES_GETSTARTED_STM32BLINKY_SRC_LAYERS_MSP_H_
