/**
 * @file stm32f4inputcapture.h
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32F4_STM32F4INPUTCAPTURE_H_
#define HARDWAREABSTRACTION_STM32F4_STM32F4INPUTCAPTURE_H_

#include <HardwareAbstraction/Stm32/stm32inputcapture.h>

#if defined(STM32F4) && defined(HAL_TIM_MODULE_ENABLED)
namespace semf
{
class Stm32F4InputCapture : public Stm32InputCapture
{
public:
	using Stm32InputCapture::Stm32InputCapture;
};

} /* namespace semf */

#endif

#endif /* HARDWAREABSTRACTION_STM32F4_STM32F4INPUTCAPTURE_H_ */
