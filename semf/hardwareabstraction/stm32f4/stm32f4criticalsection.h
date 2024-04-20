/**
 * @file stm32f4criticalsection.h
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4CRITICALSECTION_H_
#define SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4CRITICALSECTION_H_

#include <semf/hardwareabstraction/stm32/stm32criticalsection.h>

#if defined(STM32F4)
namespace semf
{
/**
 * @brief Specialization of \c Stm32CriticalSection for STM32F4.
 */
class Stm32F4CriticalSection : public Stm32CriticalSection
{
public:
	using Stm32CriticalSection::Stm32CriticalSection;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4CRITICALSECTION_H_ */
