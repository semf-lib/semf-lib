/**
 * @file myapp.h
 * @date 12.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef EXAMPLES_INPUT_SENSOR_SRC_LAYERS_APP_MYAPP_H_
#define EXAMPLES_INPUT_SENSOR_SRC_LAYERS_APP_MYAPP_H_

#include <semf/app/input/sensor/sensor.h>
#include <semf/utils/core/signals/slot.h>

namespace app
{
class MyApp
{
public:
	MyApp(semf::app::Sensor<double>& temperatureSensor, semf::app::Sensor<double>& humiditySensor);
	explicit MyApp(const MyApp& other) = delete;
	virtual ~MyApp() = default;

private:
	void onTemperatureAvailable();
	void onHumidityAvailable();

	semf::app::Sensor<double>& m_temperatureSensor;
	semf::app::Sensor<double>& m_humiditySensor;
	SEMF_SLOT(m_onTemperatureAvailableSlot, MyApp, *this, onTemperatureAvailable);
	SEMF_SLOT(m_onHumidityAvailableSlot, MyApp, *this, onHumidityAvailable);
};
}  // namespace app
#endif  // EXAMPLES_INPUT_SENSOR_SRC_LAYERS_APP_MYAPP_H_
