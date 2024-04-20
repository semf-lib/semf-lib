/**
 * @file stm32f4analogout.h
 * @date 06.08.2020
 * @author sga
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGOUT_H_
#define SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGOUT_H_

#include <semf/hardwareabstraction/stm32/stm32analogout.h>

#if defined(STM32F4) && defined(HAL_DAC_MODULE_ENABLED)
namespace semf
{
/**
 * @brief Specialization of \c Stm32AnalogOut for STM32F4.
 */
class Stm32F4AnalogOut : public Stm32AnalogOut
{
public:
	using Stm32AnalogOut::Stm32AnalogOut;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGOUT_H_ */
