/**
 * @file softpwm.h
 *
 * @date 30.09.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef OUTPUT_SOFTPWM_H_
#define OUTPUT_SOFTPWM_H_

#include <Interfaces/gpio.h>
#include <Interfaces/pwm.h>
#include <Output/digitalout.h>
#include <System/softwaretimer.h>
#include <System/timebase.h>
#include <limits>

namespace semf
{
/**
 * @brief The \c SoftPwm class outputs a PWM signal on the given GPIO.
 *
 * The timing of the PWM is controlled by an internal \c SoftwareTimer.
 * The cycle time of a PWM can can be set by \c setMaxValue() function and lasts,
 * maxValue x time base cycle time. The duty cycle can be set by \c set() function.
 */
class SoftPwm : public Pwm
{
public:
	/**
	 * @brief Constructor.
	 * @param output GPIO to output the \c SoftPwm.
	 * @param timebase \c TimeBase used for internal \c SoftwareTimer.
	 * @param inverted \c true for inverted PWM signal on the output pin.
	 */
	SoftPwm(Gpio& output, TimeBase& timebase, bool inverted = false);

	virtual ~SoftPwm() = default;
	void setMaxValue(unsigned int maxValue);
	unsigned int maxValue() const;
	void set(unsigned int value, bool inverted = false);
	unsigned int value() const;
	void start();
	void stop();
	bool isEnabled() const;

private:
	/**
	 * @brief Starts doing the on time.
	 * @note If pin is inverted, level is low.
	 */
	void startOnTime();

	/**
	 * @brief Starts doing the off time.
	 * @note If pin is inverted, level is high.
	 */
	void startOffTime();

	/**
	 * @brief Checks PWM setting for constant on / off.
	 * @return \c true for constant output level set.
	 */
	bool checkForConstantSetting();

	/**The output pin which is connected to the led.*/
	DigitalOut m_out;
	/**The timer which is used for time settings.*/
	SoftwareTimer m_pwmCycleTimer;
	/**PWM value setting.*/
	unsigned int m_pwmValue = 0;
	/**Maximal possible pwm setting.*/
	unsigned int m_maxValue = std::numeric_limits<unsigned int>::max();
};

} /* namespace semf */

#endif /* OUTPUT_SOFTPWM_H_ */
