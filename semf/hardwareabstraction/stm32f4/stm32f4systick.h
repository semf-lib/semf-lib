/**
 * @file stm32f4systick.h
 * @date 31.01.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4SYSTICK_H_
#define SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4SYSTICK_H_

#include <semf/hardwareabstraction/stm32/stm32systick.h>

#if defined(STM32F4)
namespace semf
{
/**
 * @brief Specialization of \c Stm32Systick for STM32F4.
 */
class Stm32F4Systick : public Stm32Systick<Stm32F4Systick>
{
private:
	friend class Stm32Systick<Stm32F4Systick>;
	Stm32F4Systick() = default;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4SYSTICK_H_ */
