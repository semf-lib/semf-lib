/**
 * @file temperaturesensor.h
 * @date 12.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef EXAMPLES_INPUT_SENSOR_SRC_LAYERS_BSP_TEMPERATURE_H_
#define EXAMPLES_INPUT_SENSOR_SRC_LAYERS_BSP_TEMPERATURE_H_

#include <semf/input/sensor/sensorconverter.h>

namespace bsp
{
/**
 * @brief Sensor class converting sensor data from an ADC value to a double temperature value.
 */
class Temperature : public semf::SensorConverter<uint16_t, double>
{
public:
	using semf::SensorConverter<uint16_t, double>::SensorConverter;
	void setFactor(double factor);
	void setOffset(double offset);

protected:
	double convert(uint16_t rawValue) const override;

private:
	double m_factor = 0.05;
	double m_offset = 20.0;
};
}  // namespace bsp
#endif  // EXAMPLES_INPUT_SENSOR_SRC_LAYERS_BSP_TEMPERATURE_H_
