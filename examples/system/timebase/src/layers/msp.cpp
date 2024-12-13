/**
 * @file msp.cpp
 * @date 18.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <msp.h>

namespace msp
{
Msp::Msp()
: m_timer1s(1),
  m_timer100ms(10)
{
}

void Msp::init()
{
	m_timer1s.start();
	m_timer100ms.start();
}

void Msp::deinit()
{
	m_timer1s.stop();
	m_timer100ms.stop();
}

semf::VirtualTimer& Msp::timer1s()
{
	return m_timer1s;
}

semf::VirtualTimer& Msp::systick100ms()
{
	return m_timer100ms;
}

semf::VirtualAnalogOut& Msp::analogOut()
{
	return m_analogOut;
}
}  // namespace msp
