/**
 * @file bsp.cpp
 * @date 12.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <bsp.h>

namespace bsp
{
Bsp::Bsp(msp::Msp& msp)
: m_temperaureSensorHardware(msp.analogInTemperatureSensor(), m_temperatureSensorBuffer, kSensorBufferSize),
  m_temperature(m_temperaureSensorHardware, 0),
  m_humiditySensorHardware(msp.analogInHumiditySensor(), m_HumiditySensorBuffer, kSensorBufferSize),
  m_humidity(m_humiditySensorHardware, 0, [](uint16_t value) { return value / 100.0; })
{
}

Temperature& Bsp::temperature()
{
	return m_temperature;
}

semf::SimpleSensorConverter<uint16_t, double>& Bsp::humidity()
{
	return m_humidity;
}

}  // namespace bsp
