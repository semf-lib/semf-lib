/**
 * @file stm32timer.h
 *
 * @date 31.01.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32_STM32TIMER_H_
#define HARDWAREABSTRACTION_STM32_STM32TIMER_H_

#include <HardwareAbstraction/Stm32/stm32.h>
#include <Core/Queues/linkedqueue.h>

#if defined(STM32) && defined(HAL_TIM_MODULE_ENABLED)
#include <Interfaces/timer.h>
namespace semf
{
class Stm32Timer : public Timer, public LinkedQueue<Stm32Timer>::Node
{
public:
	/**
	 * @brief Error codes for this class. Error ID identify a unique error() / onError call (excluding transferring).
	 */
	enum class ErrorCode : uint8_t
	{
		Start_HalError = 0,
		Start_HalBusy,
		Start_HalTimeout,
		Stop_HalError,
		Stop_HalBusy,
		Stop_HalTimeout,
	};
	/**
	 * @brief Constructor.
	 * @param hwHandle Hardware handler.
	 */
	explicit Stm32Timer(TIM_HandleTypeDef& hwHandle);
	/**
	 * @copydoc Timer::start()
	 * @throws Start_HalError If \c HAL_TIM_Base_Start_IT() returns HAL_ERROR.
	 * @throws Start_HalBusy If \c HAL_TIM_Base_Start_IT() returns HAL_BUSY.
	 * @throws Start_HalTimeout If \c HAL_TIM_Base_Start_IT() returns HAL_TIMEOUT.
	 */
	void start() override;
	/**
	 * @copydoc Timer::stop()
	 * @throws Stop_HalError If \c HAL_TIM_Base_Stop_IT() returns HAL_ERROR.
	 * @throws Stop_HalBusy If \c HAL_TIM_Base_Stop_IT() returns HAL_BUSY.
	 * @throws Stop_HalTimeout If \c HAL_TIM_Base_Stop_IT() returns HAL_TIMEOUT.
	 */
	void stop() override;
	void reset() override;
	/**
	 * @brief Get the list with all timers.
	 * @return The list with all timers.
	 */
	static LinkedQueue<Stm32Timer>* queue();

	/**
	 * @brief System-wide interrupt service routine for adc conversion finished.
	 * @param hwHandle Hardware handler.
	 */
	static void systemIsr(TIM_HandleTypeDef& hwHandle);

	/**
	 * @brief Interrupt service routine for adc conversion finished.
	 * @param hwHandle Hardware handler.
	 */
	void isr(TIM_HandleTypeDef& hwHandle);

private:
    /**List for all adc*/
    static LinkedQueue<Stm32Timer> m_queue;
    /**Hardware handler*/
    TIM_HandleTypeDef* m_hwHandle;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32Timer;
};

} /* namespace semf */

#endif

#endif /* HARDWAREABSTRACTION_STM32_STM32TIMER_H_ */
