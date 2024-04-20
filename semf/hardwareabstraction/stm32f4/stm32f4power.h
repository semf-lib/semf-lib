/**
 * @file stm32f4power.h
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4POWER_H_
#define SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4POWER_H_

#include <semf/hardwareabstraction/stm32/stm32power.h>

#if defined(STM32F4)
namespace semf
{
/**
 * @brief Specialization of \c Stm32Power for STM32F4.
 */
class Stm32F4Power: public Stm32Power
{
public:
	//! @cond Doxygen_Suppress
	using Stm32Power::Stm32Power;
	//! @endcond
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4POWER_H_ */
