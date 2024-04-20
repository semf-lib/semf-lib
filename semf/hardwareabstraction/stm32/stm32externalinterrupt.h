/**
 * @file stm32externalinterrupt.h
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32_STM32EXTERNALINTERRUPT_H_
#define SEMF_HARDWAREABSTRACTION_STM32_STM32EXTERNALINTERRUPT_H_

#include <semf/hardwareabstraction/stm32/stm32.h>
#include <semf/utils/core/queues/linkedqueue.h>

#if defined(STM32)
#include <semf/system/externalinterrupt.h>
#include <cstdint>

namespace semf
{
/**
 * @brief \c ExternalInterrupt implementation for STM32.
 */
class Stm32ExternalInterrupt : public ExternalInterrupt, public LinkedQueue<Stm32ExternalInterrupt>::Node
{
public:
	/**Error codes for this class. Error ID identify a unique error() / onError call (excluding transferring).*/
	enum class ErrorCode : uint8_t
	{
		SetConfigLine_HalError = 0,
		SetConfigLine_HalBusy,
		SetConfigLine_HalTimeout
	};

	/**
	 * @brief Constructor.
	 * @param externalInterruptPort Port number.
	 * @param externalInterruptPinLine Interrupt pin.
	 * @see EXTI_GPIOSel in stm32f{x}xx_hal_exti.c
	 * @see EXTI_Line in stm32f{x}xx_hal_exti.c
	 */
	explicit Stm32ExternalInterrupt(uint32_t externalInterruptPort, uint32_t externalInterruptPinLine);
	explicit Stm32ExternalInterrupt(const Stm32ExternalInterrupt& other) = delete;
	virtual ~Stm32ExternalInterrupt() = default;

	/**
	 * @brief Get the list with all external interrupts.
	 * @return The list with all external interrupts.
	 */
	static LinkedQueue<Stm32ExternalInterrupt>* queue();
	/**
	 * Must call in the interrupt callback from the stm32 hal lib.
	 * @param pin The parameter from the stm32 HAL callback function.
	 */
	static void systemIsr(uint16_t pin);
	/**
	 * Must call in the interrupt callback from the stm32 hal lib.
	 * @param pin The parameter from the stm32 HAL callback function.
	 */
	void isr(uint16_t pin);
	/**
	 * @brief Sets the trigger condition of an EXTI.
	 * @param trigger Trigger condition to set.
	 * @throws Init_HalError If the ST-HAL returns a hal error.
	 * @throws Init_HalBusy If the ST-HAL returns a hal busy.
	 * @throws Init_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void setTrigger(Trigger trigger) override;

private:
	/**Port number.*/
	uint32_t m_externalInterruptPort;
	/**Handler for the external interrupt.*/
	EXTI_HandleTypeDef m_handle;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32ExternalInterrupt;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32_STM32EXTERNALINTERRUPT_H_ */
