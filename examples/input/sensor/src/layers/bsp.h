/**
 * @file bsp.h
 * @date 12.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef EXAMPLES_INPUT_SENSOR_SRC_LAYERS_BSP_H_
#define EXAMPLES_INPUT_SENSOR_SRC_LAYERS_BSP_H_

#include <semf/input/analogin.h>
#include <semf/input/sensor/analogsensor.h>
#include <semf/input/sensor/simplesensorconverter.h>
#include <msp.h>
#include <bsp/temperature.h>

namespace bsp
{
class Bsp
{
public:
	explicit Bsp(msp::Msp& msp);
	explicit Bsp(const Bsp& other) = delete;
	virtual ~Bsp() = default;

	Temperature& temperature();
	semf::SimpleSensorConverter<uint16_t, double>& humidity();

private:
	static constexpr size_t kSensorBufferSize = 3;

	uint16_t m_temperatureSensorBuffer[kSensorBufferSize] = {0};
	semf::AnalogSensor<uint16_t, 1> m_temperaureSensorHardware;
	Temperature m_temperature;

	uint16_t m_HumiditySensorBuffer[kSensorBufferSize] = {0};
	semf::AnalogSensor<uint16_t, 1> m_humiditySensorHardware;
	semf::SimpleSensorConverter<uint16_t, double> m_humidity;
};
}  // namespace bsp
#endif  // EXAMPLES_INPUT_SENSOR_SRC_LAYERS_BSP_H_
