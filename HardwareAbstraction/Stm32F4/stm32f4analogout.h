/**
 * @file stm32f4analogout.h
 *
 * @date 06.08.2020
 * @author sga
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGOUT_H_
#define HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGOUT_H_

#include <HardwareAbstraction/Stm32/stm32analogout.h>

#if defined(STM32F4) && defined(HAL_DAC_MODULE_ENABLED)
namespace semf
{
/**
 * @attention For channel setting use DAC_CHANNEL_1 or DAC_CHANNEL_2.
 * @attention If using a hardware timer as a trigger, start it manually before starting the DMA output.
 */
class Stm32F4AnalogOut : public Stm32AnalogOut
{
public:
	using Stm32AnalogOut::Stm32AnalogOut;
};

} /* namespace semf */

#endif

#endif /* HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGOUT_H_ */
