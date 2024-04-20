/**
 * @file pwm.h
 * @date 20.04.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_OUTPUT_PWM_H_
#define SEMF_OUTPUT_PWM_H_

#include <semf/utils/core/error.h>
#include <semf/utils/core/signals/signal.h>
#include <cstdint>

namespace semf
{
/**
 * @brief Class to interface pwm's hardware module of the microcontroller.
 */
class Pwm
{
public:
	virtual ~Pwm() = default;

	/**
	 * @brief Set the maximum possible PWM value.
	 * @param maxValue The maximum possible PWM value.
	 */
	virtual void setMaxValue(unsigned int maxValue) = 0;
	/**
	 * @brief Returns the maximum possible PWM value.
	 * @return The maximum possible PWM value.
	 */
	virtual unsigned int maxValue() const = 0;
	/**
	 * @brief Set a PWM value. Duty cycle is value / maxValue.
	 * @param value The new PWM value.
	 * @param inverted Invert the PWM output.
	 */
	virtual void set(unsigned int value, bool inverted = false) = 0;
	/**
	 * @brief Returns the current PWM value.
	 * @return The current PWM value.
	 */
	virtual unsigned int value() const = 0;
	/**Start the PWM hardware.*/
	virtual void start() = 0;
	/**Stop the PWM hardware.*/
	virtual void stop() = 0;
	/**
	 * @brief Returns the current status of PWM module.
	 * @return \li true: PWM is enabled and active,
	 * \li false: PWM is disabled and not active.
	 */
	virtual bool isEnabled() const = 0;

	/**Signal is emitted after an error in the hardware occurred.*/
	Signal<Error> error;
};
} /* namespace semf */
#endif /* SEMF_OUTPUT_PWM_H_*/
