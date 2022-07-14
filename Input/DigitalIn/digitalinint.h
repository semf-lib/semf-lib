/**
 * @file digitalinint.h
 *
 * @date 22.03.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef INPUT_DIGITALIN_DIGITALININT_H_
#define INPUT_DIGITALIN_DIGITALININT_H_

#include <Input/DigitalIn/digitalin.h>
#include <Interfaces/gpio.h>
#include <Interfaces/externalinterrupt.h>

namespace semf
{
/** 
 * @brief Class for reading a digital input in interrupt mode
 * @see \c DigitalIn and \c ExternalInterrupt.
 */
class DigitalInInt : public DigitalIn
{
public:
	/**
	 * @brief Constructor.
	 * @param extInterrupt External interrupt object for hardware access.
	 * @param gpio GPIO interface object for hardware access.
	 * @param inverted \c true sets high level of a pin as low and low level of a pin as high.
	 */
	DigitalInInt(ExternalInterrupt& extInterrupt, Gpio& gpio, bool inverted = false);

	/**
	 * @brief Sets GPIO and external interrupt interface objects for hardware access.
	 * @note GPIO is needed to check if level changed to high or to low after receiving the
	 * \c changed signal from \c ExternalInterrupt object.
	 * @param extInterrupt External interrupt object for hardware access.
	 * @param gpio GPIO interface object for hardware access.
	 */
	void setExternalInterrupt(ExternalInterrupt& extInterrupt, Gpio& gpio);

private:
	/**
	 * @brief Slot for \c ExternalInterrupt changed signal.
	 */
	void onChanged();

	/**Pointer to the external interrupt interface, that accesses the hardware.*/
	ExternalInterrupt& m_extInterrupt;
};

} /* namespace semf */

#endif /* INPUT_DIGITALIN_DIGITALININT_H_ */
