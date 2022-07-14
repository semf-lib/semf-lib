/**
 * @file stm32externalinterrupt.h
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32_STM32EXTERNALINTERRUPT_H_
#define HARDWAREABSTRACTION_STM32_STM32EXTERNALINTERRUPT_H_

#include <HardwareAbstraction/Stm32/stm32.h>
#include <Core/Queues/linkedqueue.h>

#if defined(STM32)
#include <Interfaces/externalinterrupt.h>
#include <cstdint>

namespace semf
{
class Stm32ExternalInterrupt
		: public ExternalInterrupt, public LinkedQueue<Stm32ExternalInterrupt>::Node
{
public:
	/**
	 * @brief Error codes for this class. Error ID identify a unique error() / onError call (excluding transferring).
	 */
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
	 * @copydoc ExternalInterrupt:setTrigger()
	 * @throws Init_HalError If the ST-HAL returns a hal error.
	 * @throws Init_HalBusy If the ST-HAL returns a hal busy.
	 * @throws Init_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void setTrigger(Trigger trigger);
	/**Emitted when any error occurs.*/

private:
    /**List for all external interrupts.*/
    static LinkedQueue<Stm32ExternalInterrupt> m_queue;
	/**Port number.*/
    uint32_t m_externalInterruptPort;
    /**Handler for the external interrupt.*/
    EXTI_HandleTypeDef m_handle;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32ExternalInterrupt;
};

} /* namespace semf */

#endif

#endif /* HARDWAREABSTRACTION_STM32_STM32EXTERNALINTERRUPT_H_ */
