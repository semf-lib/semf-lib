/**
 * @file softpwm.h
 * @date 30.09.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_OUTPUT_SOFTPWM_H_
#define SEMF_OUTPUT_SOFTPWM_H_

#include <semf/app/system/timebase.h>
#include <semf/output/digitalout.h>
#include <semf/output/pwm.h>
#include <semf/system/gpio.h>
#include <semf/utils/core/signals/slot.h>
#include <semf/utils/system/softwaretimer.h>
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
	SoftPwm(Gpio& output, app::TimeBase& timebase, bool inverted = false);
	explicit SoftPwm(const SoftPwm& other) = delete;
	virtual ~SoftPwm() = default;

	void setMaxValue(unsigned int maxValue) override;
	unsigned int maxValue() const override;
	void set(unsigned int value, bool inverted = false) override;
	unsigned int value() const override;
	void start() override;
	void stop() override;
	bool isEnabled() const override;

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
	/**Slot for startOnTime function.*/
	SEMF_SLOT(m_startOnTimeSlot, SoftPwm, *this, startOnTime);
	/**Slot for startOffTime function.*/
	SEMF_SLOT(m_startOffTimeSlot, SoftPwm, *this, startOffTime);
	/**PWM value setting.*/
	unsigned int m_pwmValue = 0;
	/**Maximal possible pwm setting.*/
	unsigned int m_maxValue = std::numeric_limits<unsigned int>::max();
};
} /* namespace semf */
#endif /* SEMF_OUTPUT_SOFTPWM_H_ */
