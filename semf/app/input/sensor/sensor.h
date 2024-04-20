/**
 * @file sensor.h
 * @date 07.07.2022
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_APP_INPUT_SENSOR_SENSOR_H_
#define SEMF_APP_INPUT_SENSOR_SENSOR_H_

#include <semf/utils/core/signals/signal.h>

namespace semf
{
namespace app
{
template <typename T_CONVERTED>
/**
 * @brief Application class for a sensor system.
 *
 * In \c Sensor a measurement cycle can be triggered by calling \c update().
 * After completion \c dataAvailable gets emitted and the function \c currentValue() and \c averageValue()
 * can be used to acquire the measured values.
 * Multiple \c Sensor objects can be connected to one \c HardwareSensor.
 * For having a simple conversion function, \c SimpleSensorConverter can be an simple option.
 *
 * For building a sensor system, you need:
 * 1. Object from a class inheriting from \c HardwareSensor like \c AnalogSensor or \c AnalogDmaSensor.
 * 2. Data buffer.
 * 3. Object from a class inheriting from \c Sensor base class and overwriting the \c convert() function.
 *
 * @tparam T_CONVERTED Type of an application/usable sensor value which has an unit, e.g. float for temperature [°C].
 *
 * @see \c SensorConverter
 * @see \c SimpleSensorConverter
 * @see \c HardwareSensor
 */
class Sensor
{
public:
	virtual ~Sensor() = default;

	/**
	 * @brief Returns the last measured converted value.
	 * @return Converted value based on a single raw measurement.
	 */
	virtual T_CONVERTED currentValue() const = 0;
	/**
	 * @brief Returns the converted value of the average measured raw values.
	 * @return Converted value based on an average of raw measurements.
	 */
	virtual T_CONVERTED averageValue() const = 0;
	/**
	 * @brief Triggers the sensor hardware to start a measurement cycle.
	 * @note \c dataAvailable signal is emitted for new sensor data is available.
	 */
	virtual void update() = 0;

	/**Gets emitted when new sensor data is available.*/
	Signal<> dataAvailable;
};
} /* namespace app */
} /* namespace semf */
#endif /* SEMF_APP_INPUT_SENSOR_SENSOR_H_ */
