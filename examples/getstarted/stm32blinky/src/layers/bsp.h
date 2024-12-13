/**
 * @file bsp.h
 *
 * @date 09.07.2024
 * @author mm
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */
#ifndef EXAMPLES_GETSTARTED_STM32BLINKY_SRC_LAYERS_BSP_H_
#define EXAMPLES_GETSTARTED_STM32BLINKY_SRC_LAYERS_BSP_H_

#include <layers/msp.h>

#include <semf/system/timebase.h>
#include <semf/output/ledblinking.h>
#include <semf/input/digitalin/debounceddigitalinpolling.h>

#include <cstdint>

namespace bsp
{
class Bsp
{
public:
	explicit Bsp(msp::Msp& msp);
	explicit Bsp(const Bsp& other) = delete;
	virtual ~Bsp() = default;

	semf::LedBlinking& blinkingLedGreen();
	semf::DebouncedDigitalInPolling& buttonUser();

private:
	semf::TimeBase m_timeBase1ms;
	semf::LedBlinking m_blinkingLedGreen;
	semf::DebouncedDigitalInPolling m_buttonUser;

	static constexpr uint32_t kDebounceLowTime = 10U;
	static constexpr uint32_t kDebounceHighTime = 10U;
};
}  // namespace bsp
#endif  // EXAMPLES_GETSTARTED_STM32BLINKY_SRC_LAYERS_BSP_H_
