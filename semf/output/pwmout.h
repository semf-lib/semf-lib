/**
 * @file pwmout.h
 * @date 15.03.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_OUTPUT_PWMOUT_H_
#define SEMF_OUTPUT_PWMOUT_H_

#include <semf/app/output/pwmout.h>
#include <semf/output/pwm.h>

namespace semf
{
/**
 * @copydoc app::PwmOut
 */
class PwmOut : public app::PwmOut
{
public:
	/**
	 * @brief Constructor.
	 * @param pwm PWM hardware
	 */
	explicit PwmOut(Pwm& pwm);
	explicit PwmOut(const PwmOut& other) = delete;
	//! @cond Doxygen_Suppress
	virtual ~PwmOut() = default;
	//! @endcond

	void enable(bool enable = true) override;
	void disable(bool disable = true) override;
	bool isEnabled() const override;
	void start() override;
	void stop() override;
	void setValue(uint32_t pwm) override;
	uint32_t value() const override;
	void setPermille(uint16_t pwm) override;
	uint16_t permille() const override;
	void setMaxValue(uint32_t pwm) override;
	uint32_t maxValue() const override;
	void setInverted(bool inverted) override;

private:
	/**Reference to the pwm module for hardware access. */
	Pwm& m_pwm;
	/**Pwm's output inversion information.*/
	bool m_inverted = false;
};
} /* namespace semf */
#endif /* SEMF_OUTPUT_PWMOUT_H_ */
