/**
 * @file digitalout.h
 *
 * @date 15.03.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef OUTPUT_DIGITALOUT_H_
#define OUTPUT_DIGITALOUT_H_

#include <Interfaces/gpio.h>
#include <cstddef>

namespace semf
{
/** @brief Class for handling a digital output.*/
class DigitalOut
{
public:
	/** Enum for the state of the pin.*/
	enum State : bool
	{
		Low = false,  /**< Pin is set to a low state*/
		High = true  /**< Pin is set to a high state*/
	};

	/**
	 * @brief Constructor.
	 * @param gpio GPIO interface object for hardware access.
	 * @param inverted \c true sets high level of a pin as low and low level of a pin as high.
	 */
	explicit DigitalOut(Gpio& gpio, bool inverted = false);

	/**
	 * @brief Sets GPIO interface object for hardware access.
	 * @param gpio GPIO interface object for hardware access.
	 */
	void setGpio(Gpio& gpio);

	/**
	 * @brief Configures the inversion of the output.
	 * @param inverted \c true sets high level of a pin as low and a low level of a pin as high.
	 */
	void setInverted(bool inverted);

	/**
	 * @brief Sets the output level of a GPIO pin.
	 * @param state The new output level of a GPIO pin.
	 * @note Default is \c High.
	 */
	void set(State state = High);

	/**
	 * @brief Sets low level of a GPIO pin.
	 */
	void reset();

	/**
	 * @brief Toggles the output of a GPIO pin.
	 */
	void toggle();

	/**
	 * @brief Gets the present/current state of a GPIO pin.
	 * @return Present/current state of a GPIO pin.
	 */
	State state() const;

private:
	/** Pointer to GPIO interface, that accesses the hardware.*/
	Gpio& m_gpio;
	/** Enables inverting the output.*/
	bool m_inverted = false;
};

} /* namespace semf */

#endif /* OUTPUT_DIGITALOUT_H_ */
