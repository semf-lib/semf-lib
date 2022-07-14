/**
 * @file stm32pwm.h
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32_STM32PWM_H_
#define HARDWAREABSTRACTION_STM32_STM32PWM_H_

#include <HardwareAbstraction/Stm32/stm32.h>

#if defined(STM32) && defined(HAL_TIM_MODULE_ENABLED)
#include <Interfaces/pwm.h>
namespace semf
{
	/**
	 * @copydoc Pwm::Pwm()
	 * @attention Call \c setMinValue() and setMaxValue() with hardware initialization settings
	 * done e.g. in STM32CubeIDE before setting a value by \c setValue().
	 */
class Stm32Pwm : public Pwm
{
public:
	/**
	 * @brief Error codes for this class. Error ID identify a unique error() / onError call (excluding transferring).
	 */
	enum class ErrorCode : uint8_t
	{
		Set_HalError = 0,
		Set_HalBusy,
		Set_HalTimeout,
		Start_HalError,
		Start_HalBusy,
		Start_HalTimeout,
		Stop_HalError,
		Stop_HalBusy,
		Stop_HalTimeout,
	};

	/**
	 * @brief Defines the start level of a PWM cycle.
	 * See also OCxM setting IMx_CCMR1 register description.
	 * @attention The inversion option finds application here, too.
	 */
	enum class Mode: uint8_t
	{
		/**
		 *  If counting up, level is high active as long as counter value is smaller than pulse value.
		 *  If counting down, level is low active as long as counter value is smaller than pulse value.
		 */
		Mode1,
		/**
		 *  If counting up, level is low active as long as counter value is smaller than pulse value.
		 *  If counting down, level is high active as long as counter value is smaller than pulse value.
		 */
		Mode2
	};

	/**
	 * @brief Constructor.
	 * @param hwHandle Reference to timer hardware handle.
	 * @param channel Channel selection define from STM timer HAL description. Use TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3 or TIM_CHANNEL_4.
	 * @param fastMode Flag to enable or disable fast mode.
	 * @param mode \c Mode setting.
	 */
	Stm32Pwm(TIM_HandleTypeDef& hwHandle, uint32_t channel, bool fastMode = false, Mode mode = Mode::Mode1);

	void setMaxValue(unsigned int maxValue);
	unsigned int maxValue() const;
	/**
	 * @copydoc Pwm::set()
	 * @throws Set_HalError If the ST-HAL returns a hal error.
	 * @throws Set_HalBusy If the ST-HAL returns a hal busy.
	 * @throws Set_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void set(unsigned int value, bool inverted = false);
	unsigned int value() const;
	/**
	 * @copydoc Pwm::start()
	 * @throws Start_HalError If the ST-HAL returns a hal error.
	 * @throws Start_HalBusy If the ST-HAL returns a hal busy.
	 * @throws Start_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void start();
	/**
	 * @copydoc Pwm::start()
	 * @throws Stop_HalError If the ST-HAL returns a hal error.
	 * @throws Stop_HalBusy If the ST-HAL returns a hal busy.
	 * @throws Stop_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void stop();
	bool isEnabled() const;

private:
	/**Hardware handle for timer.*/
	TIM_HandleTypeDef* m_hwHandle;
	/**Timer channel used for pwm.*/
	uint32_t m_channel;
	/**Fast mode setting.*/
	bool m_fastModeEnabled;
	/**Pwm mode selection.*/
	Mode m_mode;
	/**PWM value setting.*/
	unsigned int m_pwmValue = 0;
	/**Flag if pwm is enabled.*/
	bool m_enabled = false;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32Pwm;
};

} /* namespace semf */

#endif

#endif /* HARDWAREABSTRACTION_STM32_STM32PWM_H_ */
