/**
 * @file myapp.cpp
 * @date 12.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <app/myapp.h>
#include <iostream>
#include <iomanip>

namespace app
{
MyApp::MyApp(semf::app::Sensor<double>& temperatureSensor, semf::app::Sensor<double>& humiditySensor)
: m_temperatureSensor(temperatureSensor),
  m_humiditySensor(humiditySensor)
{
	m_temperatureSensor.dataAvailable.connect(m_onTemperatureAvailableSlot);
	m_humiditySensor.dataAvailable.connect(m_onHumidityAvailableSlot);
}

void MyApp::onTemperatureAvailable()
{
	std::cout << "Temperature:\t" << std::fixed << std::setprecision(0) << m_temperatureSensor.averageValue() << "°C" << std::endl;
}

void MyApp::onHumidityAvailable()
{
	std::cout << "Humidity:\t\t" << std::fixed << std::setprecision(0) << m_humiditySensor.averageValue() << "% r.H." << std::endl;
}
}  // namespace app
