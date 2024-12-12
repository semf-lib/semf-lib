/**
 * @file digitalout.h
 * @date 06.06.2023
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_APP_OUTPUT_DIGITALOUT_H_
#define SEMF_APP_OUTPUT_DIGITALOUT_H_

#include <semf/system/gpio.h>
#include <cstddef>

namespace semf
{
namespace app
{
/**
 * @brief Class for handling a digital output.
 */
class DigitalOut
{
public:
	/** Enum for the state of the pin.*/
	enum State : bool
	{
		Low = false, /**< Pin is set to a low state*/
		High = true  /**< Pin is set to a high state*/
	};

	DigitalOut() = default;
	explicit DigitalOut(const DigitalOut& other) = delete;
	virtual ~DigitalOut() = default;

	/**
	 * @brief Configures the inversion of the output.
	 * @param inverted \c true sets high level of a pin as low and a low level of a pin as high.
	 */
	virtual void setInverted(bool inverted) = 0;
	/**
	 * @brief Sets the output level of a GPIO pin.
	 * @param state The new output level of a GPIO pin.
	 * @note Default is \c High.
	 */
	virtual void set(State state = High) = 0;
	/**Sets low level of a GPIO pin.*/
	virtual void reset() = 0;
	/**Toggles the output of a GPIO pin.*/
	virtual void toggle() = 0;
	/**
	 * @brief Gets the present/current state of a GPIO pin.
	 * @return Present/current state of a GPIO pin.
	 */
	virtual State state() const = 0;
};
} /* namespace app */
} /* namespace semf */
#endif /* SEMF_APP_OUTPUT_DIGITALOUT_H_ */
