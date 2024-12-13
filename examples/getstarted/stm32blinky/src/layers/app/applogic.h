/**
 * @file applogic.h
 *
 * @date 09.07.2024
 * @author mm
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef EXAMPLES_GETSTARTED_STM32BLINKY_SRC_LAYERS_APP_APPLOGIC_H_
#define EXAMPLES_GETSTARTED_STM32BLINKY_SRC_LAYERS_APP_APPLOGIC_H_

#include <semf/app/output/ledblinking.h>
#include <semf/app/input/digitalin/digitalin.h>

#include <semf/utils/core/signals/slot.h>

#include <cstdint>

namespace app
{
class AppLogic
{
public:
	AppLogic(semf::app::LedBlinking& blinkingLedGreen, semf::app::DigitalIn& buttonUser);
	virtual ~AppLogic() = default;

private:
	void onChangedToHigh();
	void onChangedToLow();

	SEMF_SLOT(m_onChangedToHigh, AppLogic, *this, onChangedToHigh);
	SEMF_SLOT(m_onChangedToLow, AppLogic, *this, onChangedToLow);

	semf::app::LedBlinking& m_blinkingLedGreen;
	semf::app::DigitalIn& m_buttonUser;

	static constexpr uint32_t kBlinkingOnTime = 500U;
	static constexpr uint32_t kBlinkingOffTime = 500U;
};

}  // namespace app

#endif  // EXAMPLES_GETSTARTED_STM32BLINKY_SRC_LAYERS_APP_APPLOGIC_H_
