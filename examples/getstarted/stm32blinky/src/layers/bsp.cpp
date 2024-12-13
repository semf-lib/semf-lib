/**
 * @file bsp.cpp
 *
 * @date 09.07.2024
 * @author mm
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */
#include <layers/bsp.h>

namespace bsp
{
Bsp::Bsp(msp::Msp& msp)
: m_timeBase1ms(msp.systick1ms(), true),
  m_blinkingLedGreen(msp.gpioLd3(), m_timeBase1ms),
  m_buttonUser(msp.gpioB1(), m_timeBase1ms, kDebounceLowTime, kDebounceHighTime)
{
}

semf::LedBlinking& Bsp::blinkingLedGreen()
{
	return m_blinkingLedGreen;
}

semf::DebouncedDigitalInPolling& Bsp::buttonUser()
{
	return m_buttonUser;
}
}  // namespace bsp
