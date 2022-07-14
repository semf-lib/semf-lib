/**
 * @file stm32analogininjected.h
 *
 * @date 07.10.2020
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32_STM32ANALOGININJECTED_H_
#define HARDWAREABSTRACTION_STM32_STM32ANALOGININJECTED_H_

#include <HardwareAbstraction/Stm32/stm32.h>
#include <Core/Queues/linkedqueue.h>

#if defined(STM32) && defined(HAL_ADC_MODULE_ENABLED)
#if defined(STM32F1) || defined(STM32F3) || defined(STM32F4) || defined(STM32F7)
#include <Interfaces/analogin.h>
namespace semf
{
class Stm32AnalogInInjected : public AnalogIn, public LinkedQueue<Stm32AnalogInInjected>::Node
{
public:
	/**
	 * @brief Error IDs for this class. Error ID identify a unique error() / onError call (excluding transferring).
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
	 * @param InjectedRank the ADC injected rank.
	 *		  This parameter can be one of the following values:
	 *			@arg ADC_INJECTED_RANK_1: Injected Channel1 selected
	 *			@arg ADC_INJECTED_RANK_2: Injected Channel2 selected
	 *			@arg ADC_INJECTED_RANK_3: Injected Channel3 selected
	 *			@arg ADC_INJECTED_RANK_4: Injected Channel4 selected
	 */
	Stm32AnalogInInjected(ADC_HandleTypeDef& hwHandle, uint32_t injectedRank);
	/**
	 * @copydoc AnalogIn::start()
	 * @throws Start_HalError If the ST-HAL stumbles upon an error.
	 * @throws Start_HalBusy If the ST-HAL is busy.
	 * @throws Start_HalTimeout If the ST-HAL times out.
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
	static LinkedQueue<Stm32AnalogInInjected>* queue();

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
	 * @throws ISR_ERROR_ISR If the AnalogIn-IRQ results in an error. 
	 */
	void isrError(ADC_HandleTypeDef& adc);

private:
    /**Hardware handler*/
	ADC_HandleTypeDef* m_hwHandle;
	/**Injected rank*/
	uint32_t m_injectedRank;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32AnalogInInjected;
};

} /* namespace semf */

#endif
#endif
#endif /* HARDWAREABSTRACTION_STM32_STM32ANALOGININJECTED_H_ */
