/**
 * @file leddimming.h
 *
 * @date 10.10.2018
 * @author aa
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef OUTPUT_LEDDIMMING_H_
#define OUTPUT_LEDDIMMING_H_

#include <Output/led.h>
#include <Output/pwmout.h>
#include <System/softwaretimer.h>
#include <System/timebase.h>

namespace semf
{
/**
 * @brief This class uses \c PwmOut and a \c SoftwareTimer in order to generate different waveforms.
 *        (e.g. sine, triangle), for switching the LED on and off. Additionally a pulsing mode and a
 *        constant on / off mode is available.
 *
 * \c PwmOut controls the output amplitude (brightness), while \c SoftwareTimer controls
 * the time intervals for the pulse's samples and the pulse's period.
 *
 * \c PwmOut has to be correctly configured before registering them to the class.
 *
 * The timing of the \c SoftwareTimer depends to the hardware predefined configuration
 * (e.g. clock speed, prescaler, etc.), recommended is the \c SysTick with 1ms resolution.
 *
 * Timing resolution [ms] = 1000 * Auto-reload register value of the timer / timer's input clock[Hz].
 */
class LedDimming : public Led
{
public:
	/**Led operation modes.*/
	enum class Mode : uint8_t
	{
		Off = 0,  //<! Led is off.
		On,  //<! Led is on.
		Blinking,  //<! Led is blinking.
		Sine,  //<! Led is visualizing a sine function.
		Triangule  //<! Led is visualizing a triangle function.
	};
	/**
	 * @brief Constructor.
	 * @param dimmPwmOut Reference to the \c PwmOut object used for dimming.
	 * @param timebase \c TimeBase as clock source for the internal \c SoftwareTimer.
	 */
	LedDimming(PwmOut& dimmPwmOut, TimeBase& timebase);

	virtual ~LedDimming() = default;

	/**
	 * @brief Sets the maximum brightness of the LED.
	 *
	 * Adjusts the LED brightness in blink mode and sets the maximum brightness for sine and triangle mode.
	 * There is also a function to set the minimum and maximum brightness for sine and triangle mode.
	 * @param maxBrightness Brightness value, which is between 0 and 1000.
	 */
	void setBrightness(uint16_t maxBrightness);

	/**
	 * @brief Sets the minimum and maximum brightness of the LED.
	 *
	 * For sine and triangle modes this function sets the minimum and maximum brightness.
	 * There is also a function to set only the maximum brightness, if only blinking and on / off is used.
	 * For blinking, on / off, the minBrightness is ignored.
	 * @param minBrightness Brightness value, which is between 0 and 1000.
	 * @param maxBrightness Brightness value, which is between 0 and 1000.
	 */
	void setBrightness(uint16_t minBrightness, uint16_t maxBrightness);

	/**
	 * @brief Switch on the led.
	 */
	void setOn();

	/**
	 * @brief Switch off the led.
	 */
	void setOff();

	void toggle();

	/**
	 * @brief Start blinking the led with the same on-off time.
	 *
	 * The timing of the \c SoftwareTimer module depends to the hardware
	 * predefined configuration (e.g. clock speed, prescaler, etc.), recommended is the \c SysTick with 1ms resolution.
	 *
	 * Timing resolution [ms] = 1000 * Auto-reload register value of the timer / timer's input clock[Hz].
	 *
	 * @param onOffTime On and off time.
	 */
	void setBlinking(unsigned int onOffTime);

	/**
	 * @brief Start blinking the led with different on-off time.
	 *
	 * The timing of the \c SoftwareTimer module depends to the hardware
	 * predefined configuration (e.g. clock speed, prescaler, etc.), recommended is the \c SysTick with 1ms resolution.
	 *
	 * Timing resolution [ms] = 1000 * Auto-reload register value of the timer / timer's input clock[Hz].
	 *
	 * @param onTime On time.
	 * @param offTime Off time.
	 */
	void setBlinking(unsigned int onTime, unsigned int offTime);

	/**
	 * @brief Starts showing a sine waveform on the led.
	 *
	 * Maximal brightness can be set by \c setBrightness() function. Waveform starts at \c startIndexInTimeBaseTicks.
	 * Minimal brightness is zero in these examples.
	 *
	 * * If periodInTimeBaseTicks = 1000 and startIndexInTimeBaseTicks = 0, the waveform starts at half brightness increasing afterwards.
	 * * If periodInTimeBaseTicks = 1000 and startIndexInTimeBaseTicks = 250, the waveform starts at the maximum decreasing afterwards.
	 * * If periodInTimeBaseTicks = 1000 and startIndexInTimeBaseTicks = 500, the waveform starts at half brightness decreasing afterwards.
	 * * If periodInTimeBaseTicks = 1000 and startIndexInTimeBaseTicks = 750, the waveform starts at zero increasing afterwards.
	 *
	 * @param periodInTimeBaseTicks Period of triangle is timer ticks by \c TimeBase
	 * (period time = periodInTimeBaseTicks * TimeBase timer period).
	 * @param startIndexInTimeBaseTicks index in period to start waveform.
	 */
	void setSine(unsigned int periodInTimeBaseTicks, unsigned int startIndexInTimeBaseTicks);

	/**
	 * @brief Starts showing a sine waveform on the led.
	 * @note Maximal brightness can be set by \c setBrightness() function.
	 * @note Waveform starts at half brightness setting and is increasing afterwards.
	 * @param periodInTimeBaseTicks Period of triangle is timer ticks by \c TimeBase
	 * (period time = periodInTimeBaseTicks * TimeBase timer period).
	 */
	void setSine(unsigned int periodInTimeBaseTicks);

	/**
	 * @brief Starts showing a triangle waveform on the led.
	 *
	 * Maximal brightness can be set by \c setBrightness function.
	 * Waveform starts at startIndexInTimeBaseTicks.
	 * Minimal brightness is zero in these examples.
	 *
	 * * If periodInTimeBaseTicks = 1000 and startIndexInTimeBaseTicks = 0, the waveform starts at the minimum increasing afterwards.
	 * * If periodInTimeBaseTicks = 1000 and startIndexInTimeBaseTicks = 500, the waveform starts at the maximum decreasing afterwards.
	 *
	 * @param periodInTimeBaseTicks Period of triangle is timer ticks by \c TimeBase
	 * (period time = periodInTimeBaseTicks * TimeBase timer period).
	 * @param startIndexInTimeBaseTicks index in period to start waveform.
	 */
	void setTriangle(unsigned int periodInTimeBaseTicks, unsigned int startIndexInTimeBaseTicks);

	/**
	 * @brief Starts showing a triangle waveform on the led.
	 * Maximal brightness can be set by \c setBrightness function.
	 * Waveform starts at zero and is increasing afterwards.
	 * @param periodInTimeBaseTicks Period of triangle is timer ticks by \c TimeBase
	 * (period time = periodInTimeBaseTicks * TimeBase timer period).
	 */
	void setTriangle(unsigned int periodInTimeBaseTicks);

	/**
	 * @brief Returns the active mode of the led.
	 * @return Active led mode setting.
	 */
	Mode mode() const;

private:
	/**
	 * @brief Starts doing the on time in blinking mode.
	 * @note If pin is inverted, level is low.
	 */
	void startOnTimeInBlinkingMode();

	/**
	 * @brief Starts doing the off time in blinking mode.
	 * @note If pin is inverted, level is high.
	 */
	void startOffTimeInBlinkingMode();

	/**
	 * @brief Checks PWM setting for constant on / off in blinking mode.
	 * @return \c true for constant output level set.
	 */
	bool checkForConstantSettingInBlinkingMode();

	/**
	 * @brief Sets the next sine value to the output led in sine mode.
	 */
	void nextSineValue();

	/**
	 * @brief Sets the next triangle value to the output led in sine mode.
	 */
	void nextTriangleValue();

	/**
	 * @brief This function/slot is invoked by the timeout-signal.
	 * It sets the brightness according to the previously defined settings (period, shape and sample period).
	 * @see Mode
	 */
	void onTimeout();

	/** It generates the digital output.*/
	PwmOut& m_dimmPwm;
	/** It controls time intervals for the pulse's samples and the pulse's period.*/
	SoftwareTimer m_timer;
	/** The current state of the generated output.*/
	Mode m_mode = Mode::Off;
	/** Minimum brightness value for scaling output power in sine and triangle mode. Range between 0 and 1000.*/
	uint16_t m_minBrightness = 0;
	/** Maximum brightness value for scaling output power. Range between 0 and 1000.*/
	uint16_t m_maxBrightness = 1000;
	/** The waveform completes one cycle in this period, timing depends on timer's resolution.*/
	unsigned int m_waveformPeriodTicks = 1000;
	/** Index for the current step.*/
	unsigned int m_waveformStepIndex = 0;
	/** Duration of how long LED should be on, timing depends on the timer's resolution.*/
	unsigned int m_blinkOnTicks = 0;
	/** Duration of how long LED should be off, timing depends on timer's resolution.*/
	unsigned int m_blinkOffTicks = 0;
};

}  // namespace semf

#endif /* OUTPUT_LEDDIMMING_H_ */
