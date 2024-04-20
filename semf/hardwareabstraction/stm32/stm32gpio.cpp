/**
 * @file stm32gpio.cpp
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/hardwareabstraction/stm32/stm32gpio.h>
#include <semf/utils/core/debug.h>

#if defined(STM32)
namespace semf
{
Stm32Gpio::Stm32Gpio(GPIO_TypeDef* port, uint16_t pin)
: m_port(port),
  m_pin(pin)
{
}

void Stm32Gpio::set()
{
	SEMF_INFO("set pin %u on port %p", m_pin, m_port);
	HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_SET);
}

void Stm32Gpio::reset()
{
	SEMF_INFO("reset pin %u on port %p", m_pin, m_port);
	HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_RESET);
}

bool Stm32Gpio::state() const
{
	auto state = HAL_GPIO_ReadPin(m_port, m_pin);
	if (state == GPIO_PIN_SET)
	{
		SEMF_INFO("pin %u on port %p is high", m_pin, m_port);
		return true;
	}
	else
	{
		SEMF_INFO("pin %u on port %p is low", m_pin, m_port);
		return false;
	}
}

#ifndef STM32F1
Gpio::Direction Stm32Gpio::direction() const
{
	uint16_t ioPos = pinNumberToIOPosition(m_pin);
	if (((m_port->MODER >> ioPos * 2) & 0x03) == 0)
		return Input;
	else if (((m_port->OTYPER >> ioPos) & 0x01) == 0)
		return OutputPushpull;
	else
		return OutputOpendrain;
}
#endif

#ifndef STM32F1
void Stm32Gpio::setDirection(Direction direction)
{
	uint16_t ioPos = pinNumberToIOPosition(m_pin);
	uint32_t moder = m_port->MODER;
	uint32_t otyper = m_port->OTYPER;
	moder &= ~(0x03 << (ioPos * 2));  // pin as input
	switch (direction)
	{
		case OutputPushpull:
			SEMF_INFO("set output pushpull: pin %u on port %p", m_pin, m_port);
			moder |= (1 << (ioPos * 2));
			otyper &= ~(1 << ioPos);
			break;
		case OutputOpendrain:
			SEMF_INFO("set output opendrain: pin %u on port %p", m_pin, m_port);
			moder |= (1 << (ioPos * 2));
			otyper |= (1 << ioPos);
			break;
		case Input:
			SEMF_INFO("set input: pin %u on port %p", m_pin, m_port);
			break;
		default:
			break;
	}
	m_port->MODER = moder;
	m_port->OTYPER = otyper;
}
#endif

#ifndef STM32F1
Gpio::PullUpPullDown Stm32Gpio::pullUpPullDown() const
{
	uint16_t ioPos = pinNumberToIOPosition(m_pin);
	PullUpPullDown ret = NoPullupPulldown;
	switch ((m_port->PUPDR >> (ioPos * 2)) & 0x03)
	{
		case 1:
			ret = Pullup;
			break;
		case 2:
			ret = Pulldown;
			break;
		default:
			break;
	}
	return ret;
}
#endif

#ifndef STM32F1
void Stm32Gpio::setPullUpPullDown(PullUpPullDown pullUpPullDown)
{
	uint16_t ioPos = pinNumberToIOPosition(m_pin);
	uint32_t pupdr = m_port->PUPDR;
	pupdr &= ~(0x03 << (ioPos * 2));  // no pullup, no pulldown
	switch (pullUpPullDown)
	{
		case Pullup:
			SEMF_INFO("set pullup: pin %u on port %p", m_pin, m_port);
			pupdr |= (0x01 << (ioPos * 2));
			break;
		case Pulldown:
			SEMF_INFO("set pulldown: pin %u on port %p", m_pin, m_port);
			pupdr |= (0x02 << (ioPos * 2));
			break;
		case NoPullupPulldown:
			SEMF_INFO("set no pullup / pulldown: pin %u on port %p", m_pin, m_port);
			break;
		default:
			break;
	}
	m_port->PUPDR = pupdr;
}
#endif

GPIO_TypeDef* Stm32Gpio::port() const
{
	return m_port;
}

GPIO_TypeDef* Stm32Gpio::port()
{
	return m_port;
}

uint16_t Stm32Gpio::pin() const
{
	return m_pin;
}

uint16_t Stm32Gpio::pinNumberToIOPosition(uint16_t pin) const
{
	for (uint16_t pos = 0; pos < 16; pos++)
	{
		if (pin == (1 << pos))
		{
			return pos;
		}
	}
	return 0;
}
} /* namespace semf */
#endif
