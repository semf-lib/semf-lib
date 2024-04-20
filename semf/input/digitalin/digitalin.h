/**
 * @file digitalin.h
 * @date 22.03.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_INPUT_DIGITALIN_DIGITALIN_H_
#define SEMF_INPUT_DIGITALIN_DIGITALIN_H_

#include <semf/app/input/digitalin/digitalin.h>
#include <semf/system/gpio.h>

namespace semf
{
/**
 * @brief Base class for having access to the level state of a digital input.
 * @see app::DigitalIn
 */
class DigitalIn : public app::DigitalIn
{
public:
	/**
	 * @brief Constructor.
	 * @param gpio GPIO interface object for hardware access.
	 * @param inverted \c true sets high level of a pin as low and low level of a pin as high.
	 */
	explicit DigitalIn(Gpio& gpio, bool inverted = false);
	explicit DigitalIn(const DigitalIn& other) = delete;
	//! @cond Doxygen_Suppress
	virtual ~DigitalIn() = default;
	//! @endcond

	/**
	 * @brief Returns of the pin level logic is inverted.
	 * @return \c true for inverse, otherwise \c false.
	 */
	bool isInverted() const override;
	/**
	 * @brief Configures the inversion of the input reading.
	 * @param inverted \c true interprets high level of a pin as low and a low level of a pin as high.
	 */
	void setInverted(bool inverted) override;
	/**
	 * @brief Returns the level status of the hardware pin.
	 * @note If \c DigitalIn is set inverted, state is inverted.
	 * @return \c High for high level and \c Low for low level.
	 */
	State state() const override;

protected:
	/**
	 * @brief Returns the pointer to the GPIO to check the status from.
	 * @return Pointer to the GPIO.
	 */
	Gpio& gpio() const;

private:
	/** Pointer to the GPIO interface, that accesses the hardware.*/
	Gpio* m_gpio;
	/** enables reading the input as inverted.*/
	bool m_inverted;
};
} /* namespace semf */
#endif /* SEMF_INPUT_DIGITALIN_DIGITALIN_H_ */
