/**
 * @file stm32analogin.h
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32_STM32ANALOGIN_H_
#define HARDWAREABSTRACTION_STM32_STM32ANALOGIN_H_

#include <HardwareAbstraction/Stm32/stm32.h>
#include <Core/Queues/linkedqueue.h>

#if defined(STM32) && defined(HAL_ADC_MODULE_ENABLED)
#include <Interfaces/analogin.h>
namespace semf
{
/**
 * @brief Stm32 driver for using a anologIn channel.
 * @attention You cannot call the \c start() function inside the \c dataAvailable slot. In case continous conversions are desired, you have to configure the ADC accordingly.
 */
class Stm32AnalogIn : public AnalogIn, public LinkedQueue<Stm32AnalogIn>::Node
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
		IsrError_Isr
	};
	/**
	 * @brief Constructor.
	 * @param hwHandle adc hardware interface
	 */
	explicit Stm32AnalogIn(ADC_HandleTypeDef& hwHandle);
	/**
	 * @copydoc AnalogIn::start()
	 * @throws Start_HalError If the ST-HAL stumbles upon an error.
	 * @throws Start_HalBusy If the ST-HAL is busy.
	 * @throws Start_HalTimeout If the ST-HAL times out.
	 * @attention Do not call this function inside your \c dataAvailable slot.
	 */
	virtual void start();
	virtual uint32_t value();
	/**
	 * @copydoc AnalogIn::stop()
	 * @throws Stop_HalError If the ST-HAL stumbles upon an error.
	 * @throws Stop_HalBusy If the ST-HAL is busy.
	 * @throws Stop_HalTimeout If the ST-HAL times out.
	 */
	void stop();

	/**
	 * @brief Get the list with all analog ins.
	 * @return The list with all analog ins.
	 */
	static LinkedQueue<Stm32AnalogIn>* queue();

	/**
	 * @brief System-wide interrupt service routine for adc conversion finished.
	 * @param adc Hardware handler.
	 */
	static void systemIsr(ADC_HandleTypeDef& adc);

	/**
	 * @brief System-wide interrupt service routine for adc error.
	 * @param adc Hardware handler.
	 */
	static void systemIsrError(ADC_HandleTypeDef& adc);

	/**
	 * @brief Interrupt service routine for adc conversion finished.
	 * @param adc Hardware handler.
	 */
	void isr(ADC_HandleTypeDef& adc);

	/**
	 * @brief Sending a \c error signal.
	 * @param adc Pointer to adc hardware handler.
	 * @throws IsrError_Isr If the AnalogIn-IRQ results in an error. 
	 */
	void isrError(ADC_HandleTypeDef& adc);

private:
    /**Hardware handler*/
	ADC_HandleTypeDef* m_hwHandle;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32AnalogIn;
};

} /* namespace semf */

#endif

#endif /* HARDWAREABSTRACTION_STM32_STM32ANALOGIN_H_ */
