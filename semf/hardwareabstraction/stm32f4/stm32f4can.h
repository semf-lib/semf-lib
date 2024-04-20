/**
 * @file stm32f4can.h
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4CAN_H_
#define SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4CAN_H_

#include <semf/hardwareabstraction/stm32/stm32can.h>

#if defined(STM32F4) && defined(HAL_CAN_MODULE_ENABLED)
namespace semf
{
/**
 * @brief Specialization of \c Stm32Can for STM32F4.
 */
class Stm32F4Can : public Stm32Can
{
public:
	using Stm32Can::Stm32Can;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4CAN_H_ */
