/**
 * @file pwmout.h
 * @date 06.06.2023
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_APP_OUTPUT_PWMOUT_H_
#define SEMF_APP_OUTPUT_PWMOUT_H_

#include <semf/output/pwm.h>
#include <cstdint>
#include <cstddef>

namespace semf
{
namespace app
{
/**
 * @brief Class for using a PWM as an output with permille value handling.
 */
class PwmOut
{
public:
	PwmOut() = default;
	explicit PwmOut(const PwmOut& other) = delete;
	virtual ~PwmOut() = default;

	/**
	 * @brief Enables / disable the PWM output.
	 * @param enable \li true: PWM on, \li false: PWM off.
	 */
	virtual void enable(bool enable = true) = 0;
	/**
	 * @brief Enables / disable the PWM output.
	 * @param disable \li true: PWM off, \li false: PWM on.
	 */
	virtual void disable(bool disable = true) = 0;
	/**
	 * @brief Current status of the PWM module.
	 * @return \li true: PWM on, \li false: PWM off.
	 */
	virtual bool isEnabled() const = 0;
	/**
	 * @brief Enables the PWM.
	 * @see setEnable()
	 */
	virtual void start() = 0;
	/**
	 * @brief Disable the PWM.
	 * @see setEnable()
	 */
	virtual void stop() = 0;
	/**
	 * @brief Change the PWM value.
	 * @param pwm the new PWM value.
	 */
	virtual void setValue(uint32_t pwm) = 0;
	/**
	 * @brief Returns the current PWM value.
	 * @return The PWM value.
	 */
	virtual uint32_t value() const = 0;
	/**
	 * @brief Set the PWM value in permille.
	 * The pwm value is calculated based on the set maximum value.
	 * @param pwm The PWM value in permille. Valid range is 0 - 1000.
	 * @see setMaxValue()
	 */
	virtual void setPermille(uint16_t pwm) = 0;
	/**
	 * @brief Returns the current PWM value in permille.
	 * The PWM value is calculated based on the set maximum value.
	 * @return The PWM value in permille (0 - 1000).
	 * @see setMaxValue()
	 */
	virtual uint16_t permille() const = 0;
	/**
	 * @brief Set the maximum value for the PWM.
	 * @param pwm The maximum possible PWM value. The minimum value is 1.
	 */
	virtual void setMaxValue(uint32_t pwm) = 0;
	/**
	 * @brief Returns the maximum possible PWM value.
	 * @return The maximum possible PWM value.
	 * @see setMaxValue()
	 */
	virtual uint32_t maxValue() const = 0;
	/**
	 * @brief To configure whether the PWM should be inverted.
	 * This configuration does not become active until the methods
	 * \c setValue(), \c setEnabled(bool enable) or \c start() have been called.
	 * @param inverted \li true: inverted, \li false: not inverted.
	 * @see \c setValue(), \c setEnabled() and \c start().
	 */
	virtual void setInverted(bool inverted) = 0;
};
} /* namespace app */
} /* namespace semf */
#endif /* SEMF_APP_OUTPUT_PWMOUT_H_ */
