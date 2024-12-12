/**
 * @file hardwaresensor.h
 * @date 29.05.2023
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_INPUT_SENSOR_HARDWARESENSOR_H_
#define SEMF_INPUT_SENSOR_HARDWARESENSOR_H_

#include <semf/utils/core/buffer/lastinbuffer.h>
#include <semf/utils/core/signals/signal.h>

namespace semf
{
template <typename T_RAW>
/**
 * @brief Base class for sensor hardware like analog sensor.
 * By calling \c update(), a measurement cycle gets triggered and the \c dataAvailable signal will be emitted after completion.
 * The \c buffer() function provides the raw sensor data to the \c Sensor class. By using the \c LastInBuffer class the access is thread save.
 * Multiple \c Sensor respectively \c SensorConverter objects can be connected to one \c HardwareSensor.
 * @tparam T_RAW Type of a raw sensor value, e.g. uint16_t for a 16-bit ADC value.
 *
 * @see \c AnalogSensor
 * @see \c AnalogDmaSensor
 * @see \c Sensor
 */
class HardwareSensor
{
public:
	HardwareSensor() = default;
	explicit HardwareSensor(const HardwareSensor& other) = delete;
	virtual ~HardwareSensor() = default;

	/**
	 * @brief Starts a measurement cycle and will emit \c dataAvailable afterwards. 
	 * */
	virtual void update() = 0;
	/**
	 * @brief Starts a measurement cycle for specific channel and will emit \c dataAvailable afterwards.
	 * @param channel Channel index for update. 
	 * */
	virtual void update(size_t channel) = 0;
	/**
	 * @brief Returns the buffer, where the measurement data is stored in.
	 * @param index Index of a specific sensor.
	 * @return Buffer.
	 */
	virtual LastInBuffer<T_RAW>& buffer(size_t index) = 0;

	/**Will be emitted after a measurement cycle finished.*/
	Signal<size_t> dataAvailable;
};
} /* namespace semf */
#endif /* SEMF_INPUT_SENSOR_HARDWARESENSOR_H_ */
