/**
 * @file ledblinking.h
 * @date 30.09.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_OUTPUT_LEDBLINKING_H_
#define SEMF_OUTPUT_LEDBLINKING_H_

#include <semf/app/output/ledblinking.h>
#include <semf/output/softpwm.h>

namespace semf
{
/**
 * @copydoc app::LedBlinking
 */
class LedBlinking : public app::LedBlinking
{
public:
	/**
	 * @brief Constructor.
	 * @param ledPin Led GPIO.
	 * @param timebase \c TimeBase used for internal \c SoftwareTimer.
	 * @param inverted \c true for inverted on / off status of the led.
	 */
	LedBlinking(Gpio& ledPin, app::TimeBase& timebase, bool inverted = false);
	explicit LedBlinking(const LedBlinking& other) = delete;
	//! @cond Doxygen_Suppress
	virtual ~LedBlinking() = default;
	//! @endcond

	void setOn() override;
	void setOff() override;
	void toggle() override;
	void setBlinking(unsigned int onOffTime) override;
	void setBlinking(unsigned int onTime, unsigned int offTime) override;
	Mode mode() const override;

private:
	/**Managing on / off handling.*/
	SoftPwm m_pwm;
	/**Buffering inverted pin setting.*/
	bool m_inverted;
	/**Active operation mode of led.*/
	Mode m_mode = LedBlinking::Mode::Off;
};
} /* namespace semf */
#endif /* SEMF_OUTPUT_LEDBLINKING_H_ */
