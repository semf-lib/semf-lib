/**
 * @file digitalinint.cpp
 * @date 22.03.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/input/digitalin/digitalinint.h>
#include <semf/utils/core/debug.h>

namespace semf
{
DigitalInInt::DigitalInInt(ExternalInterrupt& extInterrupt, Gpio& gpio, bool inverted)
: DigitalIn(gpio, inverted),
  m_extInterrupt(&extInterrupt)
{
	m_extInterrupt->changed.connect(m_onChanged);
}

void DigitalInInt::onChanged()
{
	if (state() == State::High)
	{
		SEMF_INFO("changed to high");
		changedToHigh();
	}
	else
	{
		SEMF_INFO("changed to low");
		changedToLow();
	}
}
} /* namespace semf */
