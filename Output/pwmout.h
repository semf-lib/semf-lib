/**
 * @file pwmout.h
 *
 * @date 15.03.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef OUTPUT_PWMOUT_H_
#define OUTPUT_PWMOUT_H_

#include <Interfaces/pwm.h>
#include <cstdint>
#include <cstddef>

namespace semf
{
/**@brief Class for using a PWM as an output with permille value handling.*/
class PwmOut
{
public:
	/**
	 * @brief Constructor.
	 * @param pwm PWM hardware
	 */
	explicit PwmOut(Pwm& pwm);

	/**
	 * @brief Register the PWM interface for the hardware access.
	 * @param pwm PWM hardware
	 */
	void setPwm(Pwm& pwm);

	/**
	 * @brief Enables / disable the PWM output.
	 * @param enable \li true: PWM on, \li false: PWM off.
	 */
	void setEnable(bool enable = true);

	/**
	 * @brief Current status of the PWM module.
	 * @return \li true: PWM on, \li false: PWM off.
	 */
	bool isEnabled() const;

	/**
	 * @brief Enables the PWM.
	 * @see setEnable()
	 */
	void start();

	/**
	 * @brief Disable the PWM.
	 * @see setEnable()
	 */
	void stop();

	/**
	 * @brief Change the PWM value.
	 * @param pwm the new PWM value.
	 */
	void setValue(uint32_t pwm);

	/**
	 * @brief Returns the current PWM value.
	 * @return The PWM value.
	 */
	uint32_t value() const;

	/**
	 * @brief Set the PWM value in permille.
	 * The pwm value is calculated based on the set maximum value.
	 * @param pwm The PWM value in permille. Valid range is 0 - 1000.
	 * @see setMaxValue()
	 */
	void setPermille(uint16_t pwm);

	/**
	 * @brief Returns the current PWM value in permille.
	 * The PWM value is calculated based on the set maximum value.
	 * @return The PWM value in permille (0 - 1000).
	 * @see setMaxValue()
	 */
	uint16_t permille() const;

	/**
	 * @brief Set the maximum value for the PWM.
	 * @param pwm The maximum possible PWM value. The minimum value is 1.
	 */
	void setMaxValue(uint32_t pwm);

	/**
	 * @brief Returns the maximum possible PWM value.
	 * @return The maximum possible PWM value.
	 * @see setMaxValue()
	 */
	uint32_t maxValue() const;

	/**
	 * @brief To configure whether the PWM should be inverted.
	 * This configuration does not become active until the methods
	 * \c setValue(), \c setEnabled(bool enable) or \c start() have been called.
	 * @param inverted \li true: inverted, \li false: not inverted.
	 * @see \c setValue(), \c setEnabled() and \c start().
	 */
	void setInverted(bool inverted);

private:
	/** Reference to the pwm module for hardware access. */
	Pwm& m_pwm;
	/** Pwm's output inversion information.*/
	bool m_inverted = false;
};

} /* namespace semf */

#endif /* OUTPUT_PWMOUT_H_ */
