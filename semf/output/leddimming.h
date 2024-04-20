/**
 * @file leddimming.h
 * @date 10.10.2018
 * @author aa
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_OUTPUT_LEDDIMMING_H_
#define SEMF_OUTPUT_LEDDIMMING_H_

#include <semf/app/system/timebase.h>
#include <semf/app/output/leddimming.h>
#include <semf/output/pwmout.h>
#include <semf/utils/core/signals/slot.h>
#include <semf/utils/system/softwaretimer.h>

namespace semf
{
/**
 * @copydoc app::LedDimming
 */
class LedDimming : public app::LedDimming
{
public:
	/**
	 * @brief Constructor.
	 * @param dimmPwmOut Reference to the \c PwmOut object used for dimming.
	 * @param timebase \c TimeBase as clock source for the internal \c SoftwareTimer.
	 */
	LedDimming(PwmOut& dimmPwmOut, app::TimeBase& timebase);
	explicit LedDimming(const LedDimming& other) = delete;
	//! @cond Doxygen_Suppress
	virtual ~LedDimming() = default;
	//! @endcond

	void setBrightness(uint16_t maxBrightness) override;
	void setBrightness(uint16_t minBrightness, uint16_t maxBrightness) override;
	void setOn() override;
	void setOff() override;
	void toggle() override;
	void setBlinking(unsigned int onOffTime) override;
	void setBlinking(unsigned int onTime, unsigned int offTime) override;
	void setSine(unsigned int periodInTimeBaseTicks, unsigned int startIndexInTimeBaseTicks) override;
	void setSine(unsigned int periodInTimeBaseTicks) override;
	void setTriangle(unsigned int periodInTimeBaseTicks, unsigned int startIndexInTimeBaseTicks) override;
	void setTriangle(unsigned int periodInTimeBaseTicks) override;
	Mode mode() const override;

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
	/**Sets the next sine value to the output led in sine mode.*/
	void nextSineValue();
	/**Sets the next triangle value to the output led in sine mode.*/
	void nextTriangleValue();
	/**
	 * @brief This function/slot is invoked by the timeout-signal.
	 * It sets the brightness according to the previously defined settings (period, shape and sample period).
	 * @see Mode
	 */
	void onTimeout();

	/**It generates the digital output.*/
	PwmOut& m_dimmPwm;
	/**It controls time intervals for the pulse's samples and the pulse's period.*/
	SoftwareTimer m_timer;
	/**The current state of the generated output.*/
	Mode m_mode = Mode::Off;
	/**Minimum brightness value for scaling output power in sine and triangle mode. Range between 0 and 1000.*/
	uint16_t m_minBrightness = 0;
	/**Maximum brightness value for scaling output power. Range between 0 and 1000.*/
	uint16_t m_maxBrightness = 1000;
	/**The waveform completes one cycle in this period, timing depends on timer's resolution.*/
	unsigned int m_waveformPeriodTicks = 1000;
	/**Index for the current step.*/
	unsigned int m_waveformStepIndex = 0;
	/**Duration of how long LED should be on, timing depends on the timer's resolution.*/
	unsigned int m_blinkOnTicks = 0;
	/**Duration of how long LED should be off, timing depends on timer's resolution.*/
	unsigned int m_blinkOffTicks = 0;
	/**Slot for startOnTimeInBlinkingMode function.*/
	SEMF_SLOT(m_startOnTimeInBlinkingModeSlot, LedDimming, *this, startOnTimeInBlinkingMode);
	/**Slot for startOffTimeInBlinkingMode function.*/
	SEMF_SLOT(m_startOffTimeInBlinkingModeSlot, LedDimming, *this, startOffTimeInBlinkingMode);
	/**Slot for nextSineValue function.*/
	SEMF_SLOT(m_nextSineValueSlot, LedDimming, *this, nextSineValue);
	/**Slot for nextTriangleValue function.*/
	SEMF_SLOT(m_nextTriangleValueSlot, LedDimming, *this, nextTriangleValue);
};
} /* namespace semf */
#endif /* SEMF_OUTPUT_LEDDIMMING_H_ */
