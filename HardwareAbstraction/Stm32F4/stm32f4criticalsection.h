/**
 * @file stm32f4criticalsection.h
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32F4_STM32F4CRITICALSECTION_H_
#define HARDWAREABSTRACTION_STM32F4_STM32F4CRITICALSECTION_H_

#include <HardwareAbstraction/Stm32/stm32criticalsection.h>

#if defined(STM32F4)
namespace semf
{
class Stm32F4CriticalSection : public Stm32CriticalSection
{
public:
	using Stm32CriticalSection::Stm32CriticalSection;
};

} /* namespace semf */

#endif
#endif /* HARDWAREABSTRACTION_STM32F4_STM32F4CRITICALSECTION_H_ */
