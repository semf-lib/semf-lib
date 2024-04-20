/**
 * @file stm32power.h
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32_STM32POWER_H_
#define SEMF_HARDWAREABSTRACTION_STM32_STM32POWER_H_

#include <semf/hardwareabstraction/stm32/stm32.h>

#if defined(STM32)
#include <semf/app/system/power.h>
namespace semf
{
/**
 * @brief \c app::Power implementation for STM32.
 */
class Stm32Power : public app::Power
{
public:
	Stm32Power() = default;
	explicit Stm32Power(const Stm32Power& other) = delete;
	virtual ~Stm32Power() = default;

	void reset() override;
	void standby() override;
	void sleep() override;
	void stop() override;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32_STM32POWER_H_ */
