/**
 * @file myapp.cpp
 * @date 18.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <app/myapp.h>
#include <iostream>

namespace app
{
MyApp::MyApp(semf::app::TimeBase& timeBase1s, semf::app::TimeBase& timeBase1ms, semf::AnalogOut& analogOut)
: m_timer9s(timeBase1s, 9, true),
  m_timer2s(timeBase1ms, 20, true),
  m_timer5s(timeBase1ms, 50, true),
  m_analogOut(analogOut)
{
	m_timer9s.timeout.connect(m_timer9sSlot);
	m_timer2s.timeout.connect(m_timer2sSlot);
	m_timer5s.timeout.connect(m_timer5sSlot);
	m_a.setValue(1);
	m_b.setValue(2000);
}

void MyApp::mainTask()
{
	uint16_t value = m_a.value() + m_b.value();
	m_analogOut.setValue(value);
	std::cout << "AnalogOut: " << value << std::endl;
}

void MyApp::calculationOfA()
{
	m_a.setValue(m_a.value() + 1);
	std::cout << "A changed to: " << m_a.value() << std::endl;
}

void MyApp::calculationOfB()
{
	m_b.setValue(m_b.value() - 100);
	std::cout << "B changed to: " << m_b.value() << std::endl;
}
}  // namespace app
