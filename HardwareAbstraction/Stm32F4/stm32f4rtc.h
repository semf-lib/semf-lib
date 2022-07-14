/**
 * @file stm32f4rtc.h
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32F4_STM32F4RTC_H_
#define HARDWAREABSTRACTION_STM32F4_STM32F4RTC_H_

#include <HardwareAbstraction/Stm32/stm32rtc.h>

#if defined(STM32F4) && defined(HAL_RTC_MODULE_ENABLED)
namespace semf
{
class Stm32F4Rtc : public Stm32Rtc
{
public:
	using Stm32Rtc::Stm32Rtc;
};

} /* namespace semf */

#endif

#endif /* HARDWAREABSTRACTION_STM32F4_STM32F4RTC_H_ */
