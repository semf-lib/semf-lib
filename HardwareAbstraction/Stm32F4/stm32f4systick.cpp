/**
 * @file stm32f4systick.cpp
 *
 * @date 28.05.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in 
 *            the project root directory.
 */

#include <HardwareAbstraction/Stm32F4/stm32f4systick.h>

#if defined(STM32F4)
extern "C"
void HAL_SYSTICK_Callback()
{
	semf::Stm32F4Systick::isr();
}
#endif
