/**
 * @file stm32power.cpp
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/hardwareabstraction/stm32/stm32power.h>
#include <semf/utils/core/debug.h>

#if defined(STM32)
namespace semf
{
void Stm32Power::reset()
{
	SEMF_INFO("reset");
	HAL_NVIC_SystemReset();
}

void Stm32Power::standby()
{
	SEMF_INFO("standby");
	HAL_PWR_EnterSTANDBYMode();
}

void Stm32Power::sleep()
{
	SEMF_INFO("sleep");
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

void Stm32Power::stop()
{
	SEMF_INFO("stop");
	HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
}
} /* namespace semf */
#endif
