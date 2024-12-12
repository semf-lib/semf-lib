/**
 * @file leddimming.h
 * @date 06.06.2023
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_APP_OUTPUT_LEDDIMMING_H_
#define SEMF_APP_OUTPUT_LEDDIMMING_H_

#include <semf/app/output/led.h>

namespace semf
{
namespace app
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
		Off = 0,   //<! Led is off.
		On,        //<! Led is on.
		Blinking,  //<! Led is blinking.
		Sine,      //<! Led is visualizing a sine function.
		Triangule  //<! Led is visualizing a triangle function.
	};

	LedDimming() = default;
	explicit LedDimming(const LedDimming& other) = delete;
	virtual ~LedDimming() = default;

	/**
	 * @brief Sets the maximum brightness of the LED.
	 *
	 * Adjusts the LED brightness in blink mode and sets the maximum brightness for sine and triangle mode.
	 * There is also a function to set the minimum and maximum brightness for sine and triangle mode.
	 * @param maxBrightness Brightness value, which is between 0 and 1000.
	 */
	virtual void setBrightness(uint16_t maxBrightness) = 0;
	/**
	 * @brief Sets the minimum and maximum brightness of the LED.
	 *
	 * For sine and triangle modes this function sets the minimum and maximum brightness.
	 * There is also a function to set only the maximum brightness, if only blinking and on / off is used.
	 * For blinking, on / off, the minBrightness is ignored.
	 * @param minBrightness Brightness value, which is between 0 and 1000.
	 * @param maxBrightness Brightness value, which is between 0 and 1000.
	 */
	virtual void setBrightness(uint16_t minBrightness, uint16_t maxBrightness) = 0;
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
	virtual void setBlinking(unsigned int onOffTime) = 0;
	/**
	 * @brief Starts blinking the led with different on and off time.
	 *
	 * The timing of the \c SoftwareTimer module depends to the hardware
	 * predefined configuration (e.g. clock speed, prescaler, etc.), recommended is the \c SysTick with 1ms resolution.
	 *
	 * Timing resolution [ms] = 1000 * Auto-reload register value of the timer / timer's input clock[Hz].
	 *
	 * @param onTime On time.
	 * @param offTime Off time.
	 */
	virtual void setBlinking(unsigned int onTime, unsigned int offTime) = 0;
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
	virtual void setSine(unsigned int periodInTimeBaseTicks, unsigned int startIndexInTimeBaseTicks) = 0;
	/**
	 * @brief Starts showing a sine waveform on the led.
	 * @note Maximal brightness can be set by \c setBrightness() function.
	 * @note Waveform starts at half brightness setting and is increasing afterwards.
	 * @param periodInTimeBaseTicks Period of triangle is timer ticks by \c TimeBase
	 * (period time = periodInTimeBaseTicks * TimeBase timer period).
	 */
	virtual void setSine(unsigned int periodInTimeBaseTicks) = 0;
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
	virtual void setTriangle(unsigned int periodInTimeBaseTicks, unsigned int startIndexInTimeBaseTicks) = 0;
	/**
	 * @brief Starts showing a triangle waveform on the led.
	 * Maximal brightness can be set by \c setBrightness function.
	 * Waveform starts at zero and is increasing afterwards.
	 * @param periodInTimeBaseTicks Period of triangle is timer ticks by \c TimeBase
	 * (period time = periodInTimeBaseTicks * TimeBase timer period).
	 */
	virtual void setTriangle(unsigned int periodInTimeBaseTicks) = 0;
	/**
	 * @brief Returns the active mode of the led.
	 * @return Active led mode setting.
	 */
	virtual Mode mode() const = 0;
};
} /* namespace app */
} /* namespace semf */
#endif /* SEMF_APP_OUTPUT_LEDDIMMING_H_ */
