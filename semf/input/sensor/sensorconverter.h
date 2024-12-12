/**
 * @file sensorconverter.h
 * @date 29.05.2023
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_INPUT_SENSOR_SENSORCONVERTER_H_
#define SEMF_INPUT_SENSOR_SENSORCONVERTER_H_

#include <semf/app/input/sensor/sensor.h>
#include <semf/input/sensor/hardwaresensor.h>
#include <semf/utils/core/signals/slot.h>
#include <semf/utils/processing/average.h>

namespace semf
{
template <typename T_RAW, typename T_CONVERTED>
/**
 * @brief Conversion class for raw sensor measurement values from \c HardwareSensor to
 *        converted values for \c Sensor used by application level.
 *
 * To use this base class, inherit from it and override the \c convert() function.
 * In order to trigger a new measurement the inherited \c update() function can be used.
 *
 * For building a measurement system, you need the following:
 * 1. Raw measurement data buffer.
 * 2. Object from a class inheriting from \c HardwareSensor interface.
 * 3. Own class inheriting from \c SensorValueConverter and implementing the \c convert() function.
 * 4. \c UserSensor object
 *
 * * @tparam T_RAW Type of a raw sensor value, e.g. uint16_t for a 16-bit ADC value.
 * * @tparam T_CONVERTED Type of an application/usable sensor value which has an unit, e.g. float for temperature [°C].
 *
 * @see \c Sensor
 * @see \c HardwareSensor
 */
class SensorConverter : public app::Sensor<T_CONVERTED>
{
public:
	/**
	 * @brief Constructor.
	 * @param hardwareSensor Sensor hardware object.
	 * @param index Index of the specific sensor.
	 */
	SensorConverter(HardwareSensor<T_RAW>& hardwareSensor, size_t index);
	explicit SensorConverter(const SensorConverter& other) = delete;
	virtual ~SensorConverter() = default;

	T_CONVERTED currentValue() const override;
	T_CONVERTED averageValue() const override;
	void update() override;

protected:
	/**
	 * @brief Converts a raw sensor measurement value from \c HardwareSensor into the required format.
	 * @param value Raw value.
	 * @return Converted value.
	 */
	virtual T_CONVERTED convert(T_RAW value) const = 0;

private:
	/**
	 * @brief Slot for \c HardwareSensor::dataAvailable. If \c index is is equal to its own index, the \c dataAvailable signal gets emitted.
	 * @param index Index of the specific sensor.
	 */
	void onDataAvailable(size_t index);

	/**Reference to hardware sensor for calling \c update() function.*/
	HardwareSensor<T_RAW>& m_hardwareSensor;
	/**Index of the specific sensor.*/
	size_t m_index;
	/**Slot for \c HardwareSensor dataAvailable signal.*/
	SEMF_SLOT(m_onDataAvailableSlot, SensorConverter, *this, onDataAvailable, size_t);
};

template <typename T_RAW, typename T_CONVERTED>
SensorConverter<T_RAW, T_CONVERTED>::SensorConverter(HardwareSensor<T_RAW>& hardwareSensor, size_t index)
: m_hardwareSensor(hardwareSensor),
  m_index(index)

{
	m_hardwareSensor.dataAvailable.connect(m_onDataAvailableSlot);
}

template <typename T_RAW, typename T_CONVERTED>
void SensorConverter<T_RAW, T_CONVERTED>::onDataAvailable(size_t index)
{
	if (index == m_index)
		app::Sensor<T_CONVERTED>::dataAvailable();
}

template <typename T_RAW, typename T_CONVERTED>
void SensorConverter<T_RAW, T_CONVERTED>::update()
{
	m_hardwareSensor.update();
}

template <typename T_RAW, typename T_CONVERTED>
T_CONVERTED SensorConverter<T_RAW, T_CONVERTED>::currentValue() const
{
	return convert(m_hardwareSensor.buffer(m_index)[0]);
}

template <typename T_RAW, typename T_CONVERTED>
T_CONVERTED SensorConverter<T_RAW, T_CONVERTED>::averageValue() const
{
	return convert(Average<T_RAW>::value(m_hardwareSensor.buffer(m_index)));
}
} /* namespace semf */
#endif /* SEMF_INPUT_SENSOR_SENSORCONVERTER_H_ */
