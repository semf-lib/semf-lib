/**
 * @file stm32f4externalinterrupt.h
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32F4_STM32F4EXTERNALINTERRUPT_H_
#define HARDWAREABSTRACTION_STM32F4_STM32F4EXTERNALINTERRUPT_H_

#include <HardwareAbstraction/Stm32/stm32externalinterrupt.h>

#if defined(STM32F4)
namespace semf
{
class Stm32F4ExternalInterrupt : public Stm32ExternalInterrupt
{
public:
	using Stm32ExternalInterrupt::Stm32ExternalInterrupt;
};

} /* namespace semf */

#endif

#endif /* HARDWAREABSTRACTION_STM32F4_STM32F4EXTERNALINTERRUPT_H_ */
