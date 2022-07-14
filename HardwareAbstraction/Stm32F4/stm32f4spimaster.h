/**
 * @file stm32f4spimaster.h
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32F4_STM32F4SPIMASTER_H_
#define HARDWAREABSTRACTION_STM32F4_STM32F4SPIMASTER_H_

#include <HardwareAbstraction/Stm32/stm32spimaster.h>

#if defined(STM32F4) && defined(HAL_SPI_MODULE_ENABLED)
namespace semf
{
/**
 * @brief Class for using the spi in master mode with the Stm32F4
 */
class Stm32F4SpiMaster : public Stm32SpiMaster
{
public:
	using Stm32SpiMaster::Stm32SpiMaster;
};

} /* namespace semf */

#endif

#endif /* HARDWAREABSTRACTION_STM32F4_STM32F4SPIMASTER_H_ */
