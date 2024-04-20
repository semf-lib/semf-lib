/**
 * @file stm32f4analogin.h
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGIN_H_
#define SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGIN_H_

#include <semf/hardwareabstraction/stm32/stm32analogin.h>

#if defined(STM32F4) && defined(HAL_ADC_MODULE_ENABLED)
namespace semf
{
/**
 * @brief Specialization of \c Stm32AnalogIn for STM32F4.
 */
class Stm32F4AnalogIn : public Stm32AnalogIn
{
public:
	using Stm32AnalogIn::Stm32AnalogIn;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGIN_H_ */
