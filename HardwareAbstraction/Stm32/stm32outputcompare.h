/**
 * @file	stm32outputcompare.h
 *
 * @date	12.03.2020
 * @author	aa
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32_STM32OUTPUTCOMPARE_H_
#define HARDWAREABSTRACTION_STM32_STM32OUTPUTCOMPARE_H_

#include <HardwareAbstraction/Stm32/stm32.h>
#include <Core/Queues/linkedqueue.h>

#if defined(STM32) && defined(HAL_TIM_MODULE_ENABLED)
#include <Interfaces/outputcompare.h>

namespace semf
{
class Stm32OutputCompare: public OutputCompare, public LinkedQueue<Stm32OutputCompare>::Node
{
public:
	/**
	 * @brief Error codes for this class. Error ID identify a unique error() / onError call (excluding transferring).
	 */
	enum class ErrorCode : uint8_t
	{
		SetMode_HalError = 0,
		SetMode_HalBusy,
		SetMode_HalTimeout,
		InitHardware_HalError,
		InitHardware_HalBusy,
		InitHardware_HalTimeout,
		StartHardware_HalError,
		StartHardware_HalBusy,
		StartHardware_HalTimeout,
		StopHardware_HalError,
		StopHardware_HalBusy,
		StopHardware_HalTimeout,
		SetTimerConfiguration_HalError,
		SetTimerConfiguration_HalBusy,
		SetTimerConfiguration_HalTimeout
	};
	/** Enum for the different module modes.*/
	enum Mode: uint32_t
	{
		Timing = TIM_OCMODE_TIMING, /**< No channel output (frozen), it is used for internal timing.*/
		RisingEdge = TIM_OCMODE_ACTIVE, /**< Set channel to rise edge on match.*/
		FallingEdge = TIM_OCMODE_INACTIVE,  /**< Set channel to fall edge on match.*/
		Toggle = TIM_OCMODE_TOGGLE /**< Toggle the channel output.*/
	};
	/**
	 * @brief Constructor for \c OutputCompare hardware module of \c Stm32 microcontroller.
	 * @param mode Working mode of hardware module @see Mode.
	 * @param hwHandle Hardware handler of the timer module.
	 * @param channel Output channel, if \c TIMING mode is selected, please pass channel zero of the hardware module.
	 * @param timerFrequency Base frequency of the used timer before precaling [KHz].
	 */
	explicit Stm32OutputCompare(Mode mode, TIM_HandleTypeDef& hwHandle, uint32_t channel, uint32_t timerFrequency /*khz*/);
	/**
	 * @brief Get the list with all \c OutputCompare modules.
	 * @return The list with all \c OutputCompare modules.
	 */
	static LinkedQueue<Stm32OutputCompare>* queue();
	/**
	 * @brief module-wide interrupt service routine for \c OutputCompare hardware module.
	 * @param hwHandle Hardware handler.
	 */
	static void moduleIsr(TIM_HandleTypeDef* hwHandle);
	/**
	 * @brief Changes the working mode of the module.
	 * @param mode Working mode @see Mode.
	 * @throws SetMode_HalError If the ST-HAL returns a hal error.
	 * @throws SetMode_HalBusy If the ST-HAL returns a hal busy.
	 * @throws SetMode_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void setMode(Mode mode);
protected:
	/**
	 * @brief Returns the hardware configuration structure of the \c OutputCompare module.
	 * @return Pointer to Hardware structure of the \c OutputCompare module.
	 */
	virtual TIM_OC_InitTypeDef* configurationStructure() = 0;
private:
	void isr();

	/** Constants that are used for the hardware module, and has to be respected during timing calculations.*/
	enum TimerConstants: uint32_t
	{
		MaxPrescalerValue = 65535,  /**< Maximum value that can be passed to prescaler hardware register.*/
		MinPrescalerValue = 0, /**< Minimum value that can be passed to prescaler hardware register.*/
		MaxArrValue = 65535, /**< Maximum value that can be passed to ARR hardware register.*/
		MinArrValue = 0 /**< Minimum value that can be passed to ARR hardware register.*/
	};
	/**
	 * @brief Returns the hardware handler of the \c OutputCompare module.
	 * @return Hardware handler of the \c OutputCompare module.
	 */
	TIM_HandleTypeDef* handle();
	/**
	 * @copydoc OutputCompare::initHardware()
	 * @throws InitHardware_HalError If the ST-HAL returns a hal error.
	 * @throws InitHardware_HalBusy If the ST-HAL returns a hal busy.
	 * @throws InitHardware_HalTimeout if the ST-HAL returns a hal timeout.
	 */
	void initHardware();
	/**
	 * @copydoc OutputCompare::startHardware()
	 * @throws StartHardware_HalError If the ST-HAL returns a hal error.
	 * @throws StartHardware_HalBusy If the ST-HAL returns a hal busy.
	 * @throws StartHardware_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void startHardware();
	/**
	 * @copydoc OutputCompare::stopHardware()
	 * @throws StopHardware_HalError If the ST-HAL returns a hal error.
	 * @throws StopHardware_HalBusy If the ST-HAL returns a hal busy.
	 * @throws StopHardware_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void stopHardware();
	/**
	 * @copydoc OutputCompare::setTimerConfiguration()
	 * @throws SetTimerConfiguration_HalError If the ST-HAL returns a hal error.
	 * @throws SetTimerConfiguration_HalBusy If the ST-HAL returns a hal busy.
	 * @throws SetTimerConfiguration_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void setTimerConfiguration();
	uint32_t timerInputFrequency() const;

	/**List for all \c Stm32OutputCompare modules.*/
	static LinkedQueue<Stm32OutputCompare> m_queue;

	Mode m_mode; /**< Working mode of the hardware module @see Mode.*/
	TIM_HandleTypeDef* m_hwHandle = nullptr; /**< Pointer to hardware handler of the timer module.*/
	uint32_t m_channel = 0; /**< Output channel.*/
	uint32_t m_frequencyBeforePrescaling = 0;  /**< Timer's input frequency before prescaling [KHz].*/
	uint32_t m_frequencyAfterPrescaling = 0; /**< Timer's input frequency after prescaling [Hz].*/
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32OutputCompare;
};
} /* namespace semf */
#endif
#endif /* HARDWAREABSTRACTION_STM32_STM32OUTPUTCOMPARE_H_ */
