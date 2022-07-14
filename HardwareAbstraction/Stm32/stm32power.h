/**
 * @file stm32power.h
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32_STM32POWER_H_
#define HARDWAREABSTRACTION_STM32_STM32POWER_H_

#include <HardwareAbstraction/Stm32/stm32.h>

#if defined(STM32)
#include <Interfaces/power.h>
namespace semf
{
class Stm32Power: public Power
{
public:
	void reset();
	void standby();
	void sleep();
	void stop();
};

} /* namespace semf */

#endif

#endif /* HARDWAREABSTRACTION_STM32_STM32POWER_H_ */
