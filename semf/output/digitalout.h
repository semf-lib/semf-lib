/**
 * @file digitalout.h
 * @date 15.03.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_OUTPUT_DIGITALOUT_H_
#define SEMF_OUTPUT_DIGITALOUT_H_

#include <semf/app/output/digitalout.h>
#include <semf/system/gpio.h>

namespace semf
{
/**
 * @copydoc app::DigitalOut
 */
class DigitalOut : public app::DigitalOut
{
public:
	/**
	 * @brief Constructor.
	 * @param gpio GPIO interface object for hardware access.
	 * @param inverted \c true sets high level of a pin as low and low level of a pin as high.
	 */
	explicit DigitalOut(Gpio& gpio, bool inverted = false);
	explicit DigitalOut(const DigitalOut& other) = delete;
	//! @cond Doxygen_Suppress
	virtual ~DigitalOut() = default;
	//! @endcond

	void setInverted(bool inverted) override;
	void set(State state = High) override;
	void reset() override;
	void toggle() override;
	State state() const override;

private:
	/**Pointer to GPIO interface, that accesses the hardware.*/
	Gpio& m_gpio;
	/**Enables inverting the output.*/
	bool m_inverted = false;
};
} /* namespace semf */
#endif /* SEMF_OUTPUT_DIGITALOUT_H_ */
