/**
 * @file stm32analogoutdma.h
 * @date 24.02.2020
 * @author sga
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */
#ifndef SEMF_HARDWAREABSTRACTION_STM32_STM32ANALOGOUTDMA_H_
#define SEMF_HARDWAREABSTRACTION_STM32_STM32ANALOGOUTDMA_H_

#include <semf/hardwareabstraction/stm32/stm32.h>
#include <semf/output/analogoutdma.h>
#include <semf/utils/core/queues/linkedqueue.h>

#if defined(STM32) && defined(HAL_DAC_MODULE_ENABLED)
namespace semf
{
/**
 * @attention For channel setting use DAC_CHANNEL_1 or DAC_CHANNEL_2.
 * @attention If using a hardware timer as a trigger, start it manually before starting the DMA output.
 */
class Stm32AnalogOutDma : public AnalogOutDma, public LinkedQueue<Stm32AnalogOutDma>::Node
{
public:
	/**Error codes for this class. Error ID identify a unique error() / onError call (excluding transferring).*/
	enum class ErrorCode : uint8_t
	{
		Start_DataInvalid = 0,
		Start_HalError,
		Start_HalBusy,
		Start_HalTimeout,
		Stop_DataInvalid,
		Stop_HalError,
		Stop_HalBusy,
		Stop_HalTimeout,
		IsrError_Isr
	};
	/**Alignment settings support by the MCU.*/
	enum class Alignment : uint32_t
	{
		Align_8bitRight = 0,
		Align_12bitLeft,
		Align_12bitRight
	};

	/**
	 * @brief Constructor.
	 * @param hwHandle Reference to hardware handle.
	 * @param channel Dma channel, please see STM documentation. Value is forwarded to the HAL drivers.
	 * @param alignment Alignment setting for channel.
	 */
	Stm32AnalogOutDma(DAC_HandleTypeDef& hwHandle, uint32_t channel, Alignment alignment);
	explicit Stm32AnalogOutDma(const Stm32AnalogOutDma& other) = delete;
	virtual ~Stm32AnalogOutDma() = default;

	/**
	 * @copydoc AnalogOut::start()
	 * @throws Start_DataInvalid If the data is invalid (nullptr or size = 0).
	 * @throws Start_HalError If the ST-HAL stumbles upon an error.
	 * @throws Start_HalBusy If the ST-HAL is busy.
	 * @throws Start_HalTimeout If the ST-HAL times out.
	 */
	void start() const override;
	/**
	 * @copydoc AnalogOut::stop()
	 * @throws Stop_DataInvalid If the data is invalid (nullptr or size = 0).
	 * @throws Stop_HalError If the ST-HAL stumbles upon an error.
	 * @throws Stop_HalBusy If the ST-HAL is busy.
	 * @throws Stop_HalTimeout If the ST-HAL times out.
	 */
	void stop() const override;
	void setData(const uint8_t data[], size_t dataSize) override;
	/**
	 * @brief Get the list with all analog outs.
	 * @return The list with all analog outs.
	 */
	static LinkedQueue<Stm32AnalogOutDma>* queue();
	/**
	 * @brief System-wide interrupt service routine for dac error.
	 * @param dac Hardware handler.
	 */
	static void systemIsrError(DAC_HandleTypeDef& dac);
	/**
	 * @brief System-wide interrupt service routine for dac conversion finished.
	 * @param dac Hardware handler.
	 */
	static void systemIsr(DAC_HandleTypeDef& dac);
	/**
	 * @brief Sending a \c error signal.
	 * @param dac Pointer to dac hardware handler.
	 */
	void isrError(DAC_HandleTypeDef& dac);
	/**
	 * @brief Sending a \c error signal.
	 * @param dac Pointer to dac hardware handler.
	 * @throws IsrError_Isr If the AnalogIn-IRQ results in an error.
	 */
	void isr(DAC_HandleTypeDef& dac);

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
	/**Pointer to the data to output by calling \c start function.*/
	const uint8_t* m_data;
	/**Size of data array.*/
	size_t m_size;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32AnalogOutDma;
};
} /* namespace semf */
#endif /* STM32 & HAL_DAC_MODULE_ENABLED */
#endif /* SEMF_HARDWAREABSTRACTION_STM32_STM32ANALOGOUTDMA_H_ */
