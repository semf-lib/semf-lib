/**
 * @file analoginmanipulator.cpp
 * @date 14.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <msp/analoginmanipulator.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <cstdlib>
#include <ctime>

namespace msp
{
AnalogInManipulator::AnalogInManipulator(semf::VirtualAnalogIn& temperature, semf::VirtualAnalogIn& humidity)
: m_temperature(temperature),
  m_humidity(humidity)
{
    std::srand(std::time(nullptr));
}

void AnalogInManipulator::loop()
{
	while (1)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		setRandomValues();
		m_temperature.start();
		m_humidity.start();
	}
}

void AnalogInManipulator::setRandomValues()
{
	m_temperature.setValue(static_cast<uint16_t>(std::rand() * 4095 / RAND_MAX));
	m_humidity.setValue(static_cast<uint16_t>(std::rand() * 4095 / RAND_MAX));
}

}  // namespace msp
