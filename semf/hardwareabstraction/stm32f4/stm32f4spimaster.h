/**
 * @file stm32f4spimaster.h
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4SPIMASTER_H_
#define SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4SPIMASTER_H_

#include <semf/hardwareabstraction/stm32/stm32spimaster.h>

#if defined(STM32F4) && defined(HAL_SPI_MODULE_ENABLED)
namespace semf
{
/**
 * @brief Specialization of \c Stm32SpiMaster for STM32F4.
 */
class Stm32F4SpiMaster : public Stm32SpiMaster
{
public:
	using Stm32SpiMaster::Stm32SpiMaster;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4SPIMASTER_H_ */
