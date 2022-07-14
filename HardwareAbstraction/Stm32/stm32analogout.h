/**
 * @file stm32analogout.h
 *
 * @date 24.02.2020
 * @author sga
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */
#ifndef HARDWAREABSTRACTION_STM32_STM32ANALOGOUT_H_
#define HARDWAREABSTRACTION_STM32_STM32ANALOGOUT_H_

#include <HardwareAbstraction/Stm32/stm32.h>
#include <Core/Queues/linkedqueue.h>

#if defined(STM32) && defined(HAL_DAC_MODULE_ENABLED)
#include <Interfaces/analogout.h>
namespace semf
{
/**
 * @attention For channel setting use DAC_CHANNEL_1 or DAC_CHANNEL_2.
 * @attention If using a hardware timer as a trigger, start it manually before starting the DMA output.
 */
class Stm32AnalogOut : public AnalogOut, public LinkedQueue<Stm32AnalogOut>::Node
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
		SetValue_HalError,
		SetValue_HalBusy,
		SetValue_HalTimeout,
		IsrError_Isr
	};
	/**Alignment settings support by the MCU.*/
	enum class Alignment : uint32_t
	{
		Align8bitRight = 0,
		Align12bitLeft,
		Align12bitRight
	};

	/**
	 * @brief Constructor.
	 * @param hdac Reference to hardware handle.
	 * @param channel Dma channel, please see STM documentation. Value is forwarded to the HAL drivers.
	 * @param alignment Alignment setting for channel.
	 */
	Stm32AnalogOut(DAC_HandleTypeDef& hwHandle, uint32_t channel, Alignment alignment);
	virtual ~Stm32AnalogOut() = default;
	/**
	 * @copydoc AnalogOut::start()
	 * @throws Start_HalError If the ST-HAL stumbles upon an error.
	 * @throws Start_HalBusy If the ST-HAL is busy.
	 * @throws Start_HalTimeout If the ST-HAL times out.
	 */
	void start() const;
	/**
	 * @copydoc AnalogOut::stop()
	 * @throws Stop_HalError If the ST-HAL stumbles upon an error.
	 * @throws Stop_HalBusy If the ST-HAL is busy.
	 * @throws Stop_HalTimeout If the ST-HAL times out.
	 */
	void stop() const;
	void setValue(uint32_t value);

	/**
	 * @brief Get the list with all analog outs.
	 * @return The list with all analog outs.
	 */
	static LinkedQueue<Stm32AnalogOut>* queue();

	/**
	 * @brief System-wide interrupt service routine for dac error.
	 * @param dac Hardware handler.
	 */
	static void systemIsrError(DAC_HandleTypeDef& dac);

	/**
	 * @brief Sending a \c error signal.
	 * @param dac Pointer to dac hardware handler.
	 * @throws IsrError_Isr If the AnalogIn-IRQ results in an error. 
	 */
	void isrError(DAC_HandleTypeDef& dac);

private:
	/**
	 * @brief Returns the alignment setting in STM format.
	 * @param setting Generic alignment setting.
	 * @return STM specific Alignment setting.
	 */
	uint32_t alignment(Alignment setting) const;

	/**Hardware handle.*/
	DAC_HandleTypeDef& m_hwHandle;
	/**Output channel*/
	uint32_t m_channel;
	/**Channel alignment setting.*/
	Alignment m_alignment;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32AnalogOut;
};

} /* namespace semf */

#endif /* STM32 & HAL_DAC_MODULE_ENABLED */
#endif /* HARDWAREABSTRACTION_STM32_STM32ANALOGOUT_H_ */
