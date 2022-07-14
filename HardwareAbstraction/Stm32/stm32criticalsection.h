/**
 * @file stm32criticalsection.h
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32_STM32CRITICALSECTION_H_
#define HARDWAREABSTRACTION_STM32_STM32CRITICALSECTION_H_

#include <HardwareAbstraction/Stm32/stm32.h>

#if defined(STM32)
#include <Interfaces/criticalsection.h>
#include <cstdint>

namespace semf
{
class Stm32CriticalSection : public CriticalSection
{
public:
	/**
	 * @brief Constructor
	 */
	using CriticalSection::CriticalSection;
	void halEnter();
	void halExit();

private:
	uint32_t m_criticalCount = 0;
};

} /* namespace semf */

#endif
#endif /* HARDWAREABSTRACTION_STM32_STM32CRITICALSECTION_H_ */
