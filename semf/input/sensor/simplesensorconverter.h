/**
 * @file simplesensorconverter.h
 * @date 29.05.2023
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_INPUT_SENSOR_SIMPLESENSORCONVERTER_H_
#define SEMF_INPUT_SENSOR_SIMPLESENSORCONVERTER_H_

#include <semf/input/sensor/sensorconverter.h>

namespace semf
{
template <typename T_RAW, typename T_CONVERTED>
/**
 * @brief Offers an simple way for implementing a \c SensorConverter class for simple conversion functions by handing over an lambda function in the
 * constructor.
 *
 * Example: SimpleSensorConverter<uint16_t, double> simpleSensorConverter(analogSensor, 0, [](uint16_t value){return value*1.5;});
 *
 * @tparam T_RAW Type of a raw sensor value, e.g. uint16_t for a 16-bit ADC value.
 * @tparam T_CONVERTED Type of an application/usable sensor value which has an unit, e.g. float for temperature [°C].
 *
 * @see \c SensorValueConverter
 * @see \c Sensor
 * @see \c HardwareSensor
 */
class SimpleSensorConverter : public SensorConverter<T_RAW, T_CONVERTED>
{
public:
	/**Typedef for lambda function pointer.*/
	typedef T_CONVERTED (*function)(T_RAW);
	/**
	 * @brief Constructor.
	 * @param hardwareSensor Sensor hardware object.
	 * @param index Index of specific sensor.
	 * @param convertFunction Lambda for converting T_RAW to T_CONVERTED via \c convert() function.
	 */
	SimpleSensorConverter(HardwareSensor<T_RAW>& hardwareSensor, size_t index, function convertFunction);
	explicit SimpleSensorConverter(const SimpleSensorConverter& other) = delete;
	virtual ~SimpleSensorConverter() = default;

private:
	T_CONVERTED convert(T_RAW value) const override;

	/**Pointer to converter function.*/
	function m_convertFunction;
};

template <typename T_RAW, typename T_CONVERTED>
SimpleSensorConverter<T_RAW, T_CONVERTED>::SimpleSensorConverter(HardwareSensor<T_RAW>& hardwareSensor, size_t index, function convertFunction)
: SensorConverter<T_RAW, T_CONVERTED>(hardwareSensor, index),
  m_convertFunction(convertFunction)

{
}

template <typename T_RAW, typename T_CONVERTED>
T_CONVERTED SimpleSensorConverter<T_RAW, T_CONVERTED>::convert(T_RAW value) const
{
	return m_convertFunction(value);
}
} /* namespace semf */
#endif /* SEMF_INPUT_SENSOR_SIMPLESENSORCONVERTER_H_ */
