/**
 * @file stm32encoder.h
 * @date 18.09.2024
 * @author vb
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32_STM32ENCODER_H_
#define SEMF_HARDWAREABSTRACTION_STM32_STM32ENCODER_H_

#include <semf/hardwareabstraction/stm32/stm32.h>
#include <semf/utils/core/queues/linkedqueue.h>
#include <semf/input/encoder.h>
#include <semf/utils/core/error.h>

#if defined(STM32) && defined(HAL_TIM_MODULE_ENABLED)

namespace semf
{
/**
 * @brief \c Encoder implementation for STM32.
 */
class Stm32Encoder : public Encoder, public LinkedQueue<Stm32Encoder>::Node
{
public:
	/**
	 * @brief Error codes for this class. Error ID identify a unique error() / onError call.
	 */
	enum class ErrorCode : uint8_t
	{
		Start_HalError = 0,
		Start_HalBusy,
		Start_HalTimeout,
		Stop_HalError,
		Stop_HalBusy,
		Stop_HalTimeout,

		Undefined,
	};

	/**
	 * @brief Constructor
	 * @param hwHandle Reference to timer hardware handle
	 */
	explicit Stm32Encoder(TIM_HandleTypeDef& hwHandle);
	explicit Stm32Encoder(const Stm32Encoder& other) = delete;
	virtual ~Stm32Encoder() = default;

	/**
	 * @copydoc Encoder::start()
	 * @throws Start_HalError If the ST-HAL stumbles upon an error.
	 * @throws Start_HalBusy If the ST-HAL is busy.
	 * @throws Start_HalTimeout If the ST-HAL times out.
	 * @throws Undefined For any other ST-HAL error.
	 */
	void start() override;
	/**
	 * @copydoc Encoder::stop()
	 * @throws Stop_HalError If the ST-HAL stumbles upon an error.
	 * @throws Stop_HalBusy If the ST-HAL is busy.
	 * @throws Stop_HalTimeout If the ST-HAL times out.
	 * @throws Undefined For any other ST-HAL error.
	 */
	void stop() override;
	/**
	 * @copydoc Encoder::position()
	 * @note The maximum value of the encoder position depends on the timer settings. Each encoder step = 4 timer steps
	 */
	uint32_t position() override;
	Direction direction() override;
	/**
	 * @brief System-wide interrupt service routine for Timer.
	 * @param hwHandle Hardware handler.
	 */
	static void systemIsr(TIM_HandleTypeDef& hwHandle);
	/**
	 * @brief Interrupt service routine.
	 * @param hwHandle Hardware handler.
	 */
	void isr(TIM_HandleTypeDef& hwHandle);

	Signal<Error> error;

private:
	/**List for all timers*/
	static LinkedQueue<Stm32Encoder> m_queue;

	/**Timer hardware handler.*/
	TIM_HandleTypeDef* m_hwHandle;
	/**Current encoder position.*/
	uint32_t m_position = 0;
	/**Last detected encoder rotate direction.*/
	Direction m_direction = Direction::Incremental;

	static constexpr uint8_t kTimerStepsProEncoderPosition = 4;

	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32Encoder;
};
}  // namespace semf
#endif
#endif  // SEMF_HARDWAREABSTRACTION_STM32_STM32ENCODER_H_
