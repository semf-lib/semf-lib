/**
 * @file stm32externalinterrupt.cpp
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <HardwareAbstraction/Stm32/stm32externalinterrupt.h>
#include <Core/debug.h>

#if defined(STM32)
extern "C"
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	semf::Stm32ExternalInterrupt::systemIsr(GPIO_Pin);
}
namespace semf
{
LinkedQueue<Stm32ExternalInterrupt> Stm32ExternalInterrupt::m_queue;
Stm32ExternalInterrupt::Stm32ExternalInterrupt(uint32_t externalInterruptPort, uint32_t externalInterruptPinLine)
	:m_externalInterruptPort(externalInterruptPort),
	 m_handle{externalInterruptPinLine, nullptr}
{
	queue()->push(*this);
}

LinkedQueue<Stm32ExternalInterrupt>* Stm32ExternalInterrupt::queue()
{
	static LinkedQueue<Stm32ExternalInterrupt> queue;
	return &queue;
}

void Stm32ExternalInterrupt::systemIsr(uint16_t pin)
{
	for (Stm32ExternalInterrupt& i : *(queue()))
		i.isr(pin);
}

void Stm32ExternalInterrupt::isr(uint16_t pin)
{
	if (pin == 0x01 << (m_handle.Line & EXTI_PIN_MASK))
	{
		SEMF_INFO("changed on pin %u", pin);
		changed();
	}
}

void Stm32ExternalInterrupt::setTrigger(Trigger trigger)
{
	EXTI_ConfigTypeDef configTypeDef;
	configTypeDef.Trigger = trigger;
	configTypeDef.Line = m_handle.Line;
	configTypeDef.Mode = EXTI_MODE_INTERRUPT;
	configTypeDef.GPIOSel = m_externalInterruptPort;

	SEMF_INFO("set trigger %u for pin on line %u", trigger, 0x01 << (m_handle.Line & EXTI_PIN_MASK));

	HAL_StatusTypeDef hal_status = HAL_EXTI_SetConfigLine(&m_handle, &configTypeDef);

	if (hal_status != HAL_OK)
	{
		if (hal_status != HAL_OK)
		{
			if (hal_status == HAL_ERROR)
			{
				SEMF_ERROR("hal error");
				error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetConfigLine_HalError)));
			}
			else if (hal_status == HAL_BUSY)
			{
				SEMF_ERROR("hal busy");
				error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetConfigLine_HalBusy)));
			}
			else if (hal_status == HAL_TIMEOUT)
			{
				SEMF_ERROR("hal timeout");
				error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetConfigLine_HalTimeout)));
			}
			return;
		}
	}
}

} /* namespace semf */

#endif
