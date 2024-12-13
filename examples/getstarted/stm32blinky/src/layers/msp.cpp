/**
 * @file msp.cpp
 *
 * @date 09.07.2024
 * @author mm
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */
#include <main.h>

#include <layers/msp.h>

namespace msp
{
Msp::Msp()
: m_gpioLd3(LD3_GPIO_Port, LD3_Pin),
  m_gpioB1(B1_GPIO_Port, B1_Pin),
  m_systick1ms(semf::Stm32F4Systick::instance())
{
}

semf::Stm32F4Gpio& Msp::gpioLd3()
{
	return m_gpioLd3;
}

semf::Stm32F4Gpio& Msp::gpioB1()
{
	return m_gpioB1;
}

semf::Stm32F4Systick& Msp::systick1ms()
{
	return m_systick1ms;
}
}  // namespace msp
