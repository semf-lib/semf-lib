/**
 * @file digitalinint.h
 * @date 22.03.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_INPUT_DIGITALIN_DIGITALININT_H_
#define SEMF_INPUT_DIGITALIN_DIGITALININT_H_

#include <semf/input/digitalin/digitalin.h>
#include <semf/system/externalinterrupt.h>
#include <semf/utils/core/signals/slot.h>

namespace semf
{
/**
 * @brief Class for reading a digital input in interrupt mode
 * @see \c app::DigitalIn
 * @see \c ExternalInterrupt.
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
	explicit DigitalInInt(const DigitalInInt& other) = delete;
	virtual ~DigitalInInt() = default;

private:
	/**Slot for \c ExternalInterrupt changed signal.*/
	void onChanged();

	/**Pointer to the external interrupt interface, that accesses the hardware.*/
	ExternalInterrupt* m_extInterrupt;
	/**Slot for onChanged function.*/
	SEMF_SLOT(m_onChanged, DigitalInInt, *this, onChanged);
};
} /* namespace semf */
#endif /* SEMF_INPUT_DIGITALIN_DIGITALININT_H_ */
