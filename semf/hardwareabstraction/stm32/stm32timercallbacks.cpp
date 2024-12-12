/**
 * @file stm32timercallbacks.cpp
 * @date 18.09.2024
 * @author vb
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/hardwareabstraction/stm32/stm32inputcapture.h>
#include <semf/hardwareabstraction/stm32/stm32encoder.h>

#if defined(STM32) && defined(HAL_TIM_MODULE_ENABLED)
extern "C" void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim)
{
	semf::Stm32InputCapture::systemIsr(*htim);
	semf::Stm32Encoder::systemIsr(*htim);
}

#endif
