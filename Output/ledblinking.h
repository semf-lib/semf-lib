/**
 * @file ledblinking.h
 *
 * @date 30.09.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef OUTPUT_LEDBLINKING_H_
#define OUTPUT_LEDBLINKING_H_

#include <Output/led.h>
#include <Output/softpwm.h>

namespace semf
{
/**
 * @brief Convenient way of switching an LED on and off or blinking.
 * The blinking is realized through a internal \c SoftwareTimer.
 */
class LedBlinking : public Led
{
public:
	/**Led operation modes.*/
	enum class Mode : uint8_t
	{
		Off = 0,  //<! Led is off.
		On = 1,  //<! Led is on.
		Blinking = 2  //<! Led is blinking.
	};
	/**
	 * @brief Constructor.
	 * @param ledPin Led GPIO.
	 * @param timebase \c TimeBase used for internal \c SoftwareTimer.
	 * @param inverted \c true for inverted on / off status of the led.
	 */
	LedBlinking(Gpio& ledPin, TimeBase& timebase, bool inverted = false);
	virtual ~LedBlinking() = default;
	void setOn();
	void setOff();
	void toggle();
	void setBlinking(unsigned int onOffTime);
	void setBlinking(unsigned int onTime, unsigned int offTime);
	/**
	 * @brief Returns the active mode of the led.
	 * @return Active led mode setting.
	 */
	Mode mode() const;

private:
	/**Managing on / off handling.*/
	SoftPwm m_pwm;
	/**Buffering inverted pin setting.*/
	bool m_inverted;
	/**Active operation mode of led.*/
	Mode m_mode = LedBlinking::Mode::Off;
};

} /* namespace semf */

#endif /* OUTPUT_LEDBLINKING_H_ */
