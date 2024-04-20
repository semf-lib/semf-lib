/**
 * @file stm32criticalsection.h
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32_STM32CRITICALSECTION_H_
#define SEMF_HARDWAREABSTRACTION_STM32_STM32CRITICALSECTION_H_

#include <semf/hardwareabstraction/stm32/stm32.h>

#if defined(STM32)
#include <semf/system/criticalsection.h>
#include <cstdint>

namespace semf
{
/**
 * @brief \c CriticalSection implementation for STM32.
 */
class Stm32CriticalSection : public CriticalSection
{
public:
	//! @cond Doxygen_Suppress
	using CriticalSection::CriticalSection;
	//! @endcond
	void halEnter() override;
	void halExit() override;

private:
	uint32_t m_criticalCount = 0;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32_STM32CRITICALSECTION_H_ */
