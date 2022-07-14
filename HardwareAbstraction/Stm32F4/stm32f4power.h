/**
 * @file stm32f4power.h
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32F4_STM32F4POWER_H_
#define HARDWAREABSTRACTION_STM32F4_STM32F4POWER_H_

#include <HardwareAbstraction/Stm32/stm32power.h>

#if defined(STM32F4)
namespace semf
{
class Stm32F4Power: public Stm32Power
{
public:
	using Stm32Power::Stm32Power;
};

} /* namespace semf */

#endif

#endif /* HARDWAREABSTRACTION_STM32F4_STM32F4POWER_H_ */
