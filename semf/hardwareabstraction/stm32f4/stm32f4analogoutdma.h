/**
 * @file stm32f4analogoutdma.h
 * @date 06.08.2020
 * @author sga
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGOUTDMA_H_
#define SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGOUTDMA_H_

#include <semf/hardwareabstraction/stm32/stm32analogoutdma.h>

#if defined(STM32F4) && defined(HAL_DAC_MODULE_ENABLED)
namespace semf
{
/**
 * @brief Specialization of \c Stm32AnalogOutDma for STM32F4.
 */
class Stm32F4AnalogOutDma : public Stm32AnalogOutDma
{
public:
	using Stm32AnalogOutDma::Stm32AnalogOutDma;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGOUTDMA_H_ */
