/**
 * @file stm32analogoutcallbacks.cpp
 * @date 07.10.2020
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/hardwareabstraction/stm32/stm32analogout.h>
#include <semf/hardwareabstraction/stm32/stm32analogoutdma.h>

#if defined(STM32) && defined(HAL_DAC_MODULE_ENABLED)
#if defined(STM32F4) || defined(STM32F7)
extern "C"
void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac)
{
	semf::Stm32AnalogOutDma::systemIsr(*hdac);
}

extern "C"
void HAL_DAC_ErrorCallbackCh1(DAC_HandleTypeDef *hdac)
{
	semf::Stm32AnalogOut::systemIsrError(*hdac);
	semf::Stm32AnalogOutDma::systemIsrError(*hdac);
}

extern "C"
void HAL_DAC_DMAUnderrunCallbackCh1(DAC_HandleTypeDef *hdac)
{
	semf::Stm32AnalogOut::systemIsrError(*hdac);
	semf::Stm32AnalogOutDma::systemIsrError(*hdac);
}
#endif
#endif
