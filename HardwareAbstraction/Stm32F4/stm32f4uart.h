/**
 * @file stm32f4uart.h
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32F4_STM32F4UART_H_
#define HARDWAREABSTRACTION_STM32F4_STM32F4UART_H_
#include <HardwareAbstraction/Stm32/stm32uart.h>

#if defined(STM32F4) && defined(HAL_UART_MODULE_ENABLED)
namespace semf
{
/**
 * @brief   Implementation of the UART interface for an STM32F4 uC.
 */
class Stm32F4Uart : public Stm32Uart
{
public:
	using Stm32Uart::Stm32Uart;
};

} /* namespace semf */

#endif

#endif /* HARDWAREABSTRACTION_STM32F4_STM32F4UART_H_ */
