/**
 * @file stm32f4spislave.h
 *
 * @date 27.01.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in 
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32F4_STM32F4SPISLAVE_H_
#define HARDWAREABSTRACTION_STM32F4_STM32F4SPISLAVE_H_

#include <HardwareAbstraction/Stm32/stm32spislave.h>

#if defined(STM32F4) && defined(HAL_SPI_MODULE_ENABLED)
namespace semf
{
/**
 * @brief Class for using the spi in slave mode with the Stm32F4
 */
class Stm32F4SpiSlave : public Stm32SpiSlave
{
public:
	using Stm32SpiSlave::Stm32SpiSlave;
};

} /* namespace semf */

#endif

#endif /* HARDWAREABSTRACTION_STM32F4_STM32F4SPISLAVE_H_ */
