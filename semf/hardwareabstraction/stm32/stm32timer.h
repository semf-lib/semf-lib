/**
 * @file stm32timer.h
 * @date 31.01.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32_STM32TIMER_H_
#define SEMF_HARDWAREABSTRACTION_STM32_STM32TIMER_H_

#include <semf/app/system/timer.h>
#include <semf/hardwareabstraction/stm32/stm32.h>
#include <semf/utils/core/queues/linkedqueue.h>

#if defined(STM32) && defined(HAL_TIM_MODULE_ENABLED)
namespace semf
{
/**
 * @brief \c Timer implementation for STM32 as peripheral timer.
 * @note This class implements \c HAL_TIM_PeriodElapsedCallback . In some cases STM32CubeIDE implements this
 *       function on its own which results in a multiple definitions linking error. In that case define
 *       \c SEMF_TIMER_USE_CUSTOM_CALLBACK and extend STM32CubeIDE's implementation accordingly.
 */
class Stm32Timer : public app::Timer, public LinkedQueue<Stm32Timer>::Node
{
public:
	/**Error codes for this class. Error ID identify a unique error() / onError call (excluding transferring).*/
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
	explicit Stm32Timer(const Stm32Timer& other) = delete;
	virtual ~Stm32Timer() = default;

	/**
	 * @copydoc app::Timer::start()
	 * @throws Start_HalError If \c HAL_TIM_Base_Start_IT() returns HAL_ERROR.
	 * @throws Start_HalBusy If \c HAL_TIM_Base_Start_IT() returns HAL_BUSY.
	 * @throws Start_HalTimeout If \c HAL_TIM_Base_Start_IT() returns HAL_TIMEOUT.
	 */
	void start() override;
	/**
	 * @copydoc app::Timer::stop()
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
	/**List for all timers*/
	static LinkedQueue<Stm32Timer> m_queue;
	/**Hardware handler*/
	TIM_HandleTypeDef* m_hwHandle;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32Timer;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32_STM32TIMER_H_ */
