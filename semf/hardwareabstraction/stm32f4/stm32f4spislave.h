/**
 * @file stm32f4spislave.h
 * @date 27.01.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in 
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4SPISLAVE_H_
#define SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4SPISLAVE_H_

#include <semf/hardwareabstraction/stm32/stm32spislave.h>

#if defined(STM32F4) && defined(HAL_SPI_MODULE_ENABLED)
namespace semf
{
/**
 * @brief Specialization of \c Stm32SpiSlave for STM32F4.
 */
class Stm32F4SpiSlave : public Stm32SpiSlave
{
public:
	using Stm32SpiSlave::Stm32SpiSlave;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4SPISLAVE_H_ */
