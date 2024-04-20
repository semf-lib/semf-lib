/**
 * @file stm32criticalsection.cpp
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 *
 */

#include <semf/hardwareabstraction/stm32/stm32criticalsection.h>
#include <semf/utils/core/debug.h>

#if defined(STM32)
namespace semf
{
void Stm32CriticalSection::halEnter()
{
	SEMF_INFO("will enter now");
	do
	{
		if (m_criticalCount++ == 0)
		{
			__disable_irq();
		}
	} while (0);
}

void Stm32CriticalSection::halExit()
{
	do
	{
		if (--m_criticalCount == 0)
		{
			__enable_irq();
		}
	} while (0);
	SEMF_INFO("left");
}
} /* namespace semf */
#endif
