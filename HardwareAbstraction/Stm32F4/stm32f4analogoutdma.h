/**
 * @file stm32f4analogoutdma.h
 *
 * @date 06.08.2020
 * @author sga
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGOUTDMA_H_
#define HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGOUTDMA_H_

#include <HardwareAbstraction/Stm32/stm32analogoutdma.h>

#if defined(STM32F4) && defined(HAL_DAC_MODULE_ENABLED)
namespace semf
{
/**
 * @attention For channel setting use DAC_CHANNEL_1 or DAC_CHANNEL_2.
 * @attention If using a hardware timer as a trigger, start it manually before starting the DMA output.
 */
class Stm32F4AnalogOutDma : public Stm32AnalogOutDma
{
public:
	using Stm32AnalogOutDma::Stm32AnalogOutDma;
};

} /* namespace semf */

#endif

#endif /* HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGOUTDMA_H_ */
