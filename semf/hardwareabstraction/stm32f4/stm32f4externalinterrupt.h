/**
 * @file stm32f4externalinterrupt.h
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4EXTERNALINTERRUPT_H_
#define SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4EXTERNALINTERRUPT_H_

#include <semf/hardwareabstraction/stm32/stm32externalinterrupt.h>

#if defined(STM32F4)
namespace semf
{
/**
 * @brief Specialization of \c Stm32ExternalInterrupt for STM32F4.
 */
class Stm32F4ExternalInterrupt : public Stm32ExternalInterrupt
{
public:
	using Stm32ExternalInterrupt::Stm32ExternalInterrupt;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4EXTERNALINTERRUPT_H_ */
