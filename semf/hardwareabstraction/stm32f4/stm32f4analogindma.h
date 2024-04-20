/**
 * @file stm32f4analogindma.h
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGINDMA_H_
#define SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGINDMA_H_

#include <semf/hardwareabstraction/stm32/stm32analogindma.h>

#if defined(STM32F4)
namespace semf
{
/**
 * @brief Specialization of \c Stm32AnalogInDma for STM32F4.
 */
class Stm32F4AnalogInDma : public Stm32AnalogInDma
{
public:
	using Stm32AnalogInDma::Stm32AnalogInDma;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4ANALOGINDMA_H_ */
