/**
 * @file stm32analogincallbacks.cpp
 * @date 07.10.2020
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/hardwareabstraction/stm32/stm32analogin.h>
#include <semf/hardwareabstraction/stm32/stm32analogindma.h>
#include <semf/hardwareabstraction/stm32/stm32analogininjected.h>

#if defined(STM32) && defined(HAL_ADC_MODULE_ENABLED)
extern "C"
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	semf::Stm32AnalogIn::systemIsr(*hadc);
	semf::Stm32AnalogInDma::systemIsr(*hadc);
}

extern "C"
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef* hadc)
{
	semf::Stm32AnalogIn::systemIsrError(*hadc);
	semf::Stm32AnalogInDma::systemIsrError(*hadc);
#if defined(STM32F1) || defined(STM32F3) || defined(STM32F4) || defined(STM32F7)
	semf::Stm32AnalogInInjected::systemIsrError(*hadc);
#endif
}

#if defined(STM32F1) || defined(STM32F3) || defined(STM32F4) || defined(STM32F7)
extern "C"
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	semf::Stm32AnalogInInjected::systemIsr(*hadc);
}
#endif
#endif
