/**
 * @file stm32inputcapture.h
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32_STM32INPUTCAPTURE_H_
#define SEMF_HARDWAREABSTRACTION_STM32_STM32INPUTCAPTURE_H_

#include <semf/hardwareabstraction/stm32/stm32.h>
#include <semf/utils/core/queues/linkedqueue.h>

#if defined(STM32) && defined(HAL_TIM_MODULE_ENABLED)
#include <semf/input/inputcapture.h>
#include <cstdint>
namespace semf
{
/**
 * @brief \c InputCapture implementation for STM32.
 */
class Stm32InputCapture : public InputCapture, public LinkedQueue<Stm32InputCapture>::Node
{
public:
	/**Error IDs for this class. Error ID identify a unique error() / onError call (excluding transferring).*/
	enum class ErrorCode : uint8_t
	{
		Start_Error = 0,
		Start_Busy,
		Start_Timeout,
		Stop_Error,
		Stop_Busy,
		Stop_Timeout,
		SetTrigger_Busy,
		SetTrigger_TriggerInvalid
	};

	/**
	 * @brief Constructor.
	 * @param hwHandle Hardware handler.
	 * @param channel Channel selection define from STM timer HAL description. Use TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3 or TIM_CHANNEL_4.
	 * @param ticksPerSecond The number of ticks per second (hardware timer frequency).
	 */
	Stm32InputCapture(TIM_HandleTypeDef& hwHandle, uint32_t channel, unsigned int ticksPerSecond);
	explicit Stm32InputCapture(const Stm32InputCapture& other) = delete;
	virtual ~Stm32InputCapture() = default;

	unsigned int ticksPerSecond() const override;
	void setMaxTicks(unsigned int maxTicks) override;
	unsigned int maxTicks() const override;
	/**
	 * @copydoc InputCapture::start()
	 * @throws Start_Error If \c HAL_TIM_IC_Start_IT returns \c HAL_ERROR.
	 * @throws Start_Busy If \c HAL_TIM_IC_Start_IT returns \c HAL_BUSY.
	 * @throws Start_Timeout If \c HAL_TIM_IC_Start_IT returns \c HAL_TIMEOUT.
	 */
	void start() override;
	/**
	 * @copydoc InputCapture::stop()
	 * @throws Stop_Error If \c HAL_TIM_IC_Stop_IT returns \c HAL_ERROR.
	 * @throws Stop_Busy If \c HAL_TIM_IC_Stop_IT returns \c HAL_BUSY.
	 * @throws Stop_Timeout If \c HAL_TIM_IC_Stop_IT returns \c HAL_TIMEOUT.
	 */
	void stop() override;
	unsigned int ticks() override;
	/**
	 * @copydoc InputCapture::start()
	 * @throws SetTrigger_Busy If \c m_hwHandle is locked.
	 * @throws SetTrigger_TriggerInvalid If \c trigger contains an invalid value.
	 */
	void setTrigger(Trigger trigger) override;
	/**
	 * @brief System-wide interrupt service routine for input capture.
	 * @param hwHandle Hardware handler.
	 */
	static void systemIsr(TIM_HandleTypeDef& hwHandle);
	/**
	 * @brief Interrupt service routine for input capture.
	 * @param hwHandle Hardware handler.
	 */
	void isr(TIM_HandleTypeDef& hwHandle);

private:
	/**List for all input captures*/
	static LinkedQueue<Stm32InputCapture> m_queue;
	/**Hardware handle*/
	TIM_HandleTypeDef* m_hwHandle;
	/**Timer channel*/
	uint32_t m_channel;
	/**Number of ticks which should be counted per second.*/
	const unsigned int m_ticksPerSecond;
	/**Class ID.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32InputCapture;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32_STM32INPUTCAPTURE_H_ */
