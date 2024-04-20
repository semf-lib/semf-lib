/**
 * @file stm32f4rtc.h
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4RTC_H_
#define SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4RTC_H_

#include <semf/hardwareabstraction/stm32/stm32rtc.h>

#if defined(STM32F4) && defined(HAL_RTC_MODULE_ENABLED)
namespace semf
{
/**
 * @brief Specialization of \c Stm32Rtc for STM32F4.
 */
class Stm32F4Rtc : public Stm32Rtc
{
public:
	using Stm32Rtc::Stm32Rtc;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4RTC_H_ */
