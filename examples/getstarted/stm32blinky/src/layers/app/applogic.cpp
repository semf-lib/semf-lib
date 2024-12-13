/**
 * @file applogic.cpp
 *
 * @date 09.07.2024
 * @author mm
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <layers/app/applogic.h>

namespace app
{
AppLogic::AppLogic(semf::app::LedBlinking& blinkingLedGreen, semf::app::DigitalIn& buttonUser)
: m_blinkingLedGreen(blinkingLedGreen),
  m_buttonUser(buttonUser)
{
	m_buttonUser.changedToHigh.connect(m_onChangedToHigh);
	m_buttonUser.changedToLow.connect(m_onChangedToLow);

	m_blinkingLedGreen.setBlinking(kBlinkingOnTime, kBlinkingOffTime);
}

void AppLogic::onChangedToHigh()
{
	m_blinkingLedGreen.setOff();
}

void AppLogic::onChangedToLow()
{
	m_blinkingLedGreen.setBlinking(kBlinkingOnTime, kBlinkingOffTime);
}

}  // namespace app
