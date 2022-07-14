/**
 * @file digitalinint.cpp
 *
 * @date 22.03.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <Input/DigitalIn/digitalinint.h>
#include <Core/debug.h>

namespace semf
{
DigitalInInt::DigitalInInt(ExternalInterrupt& extInterrupt, Gpio& gpio, bool inverted)
	:DigitalIn(gpio, inverted),
	 m_extInterrupt(extInterrupt)
{
	extInterrupt.changed.reconnect(this, &DigitalInInt::onChanged);
}

void DigitalInInt::setExternalInterrupt(ExternalInterrupt& extInterrupt, Gpio& gpio)
{
	SEMF_INFO("set new external interrupt and gpio");
	m_extInterrupt = extInterrupt;
	extInterrupt.changed.reconnect(this, &DigitalInInt::onChanged);

	DigitalIn::setGpio(gpio);
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
