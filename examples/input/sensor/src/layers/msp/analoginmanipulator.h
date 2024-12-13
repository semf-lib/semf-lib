/**
 * @file analoginmanipulator.h
 * @date 14.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef EXAMPLES_INPUT_SENSOR_SRC_LAYERS_MSP_ANALOGINMANIPULATOR_H_
#define EXAMPLES_INPUT_SENSOR_SRC_LAYERS_MSP_ANALOGINMANIPULATOR_H_

#include <semf/hardwareabstraction/virtual/virtualanalogin.h>

namespace msp
{
class AnalogInManipulator
{
public:
	AnalogInManipulator(semf::VirtualAnalogIn& temperature, semf::VirtualAnalogIn& humidity);
	explicit AnalogInManipulator(const semf::VirtualAnalogIn& other) = delete;
	virtual ~AnalogInManipulator() = default;

	void loop();

private:
	void setRandomValues();

	semf::VirtualAnalogIn& m_temperature;
	semf::VirtualAnalogIn& m_humidity;
};
}  // namespace msp
#endif  // EXAMPLES_INPUT_SENSOR_SRC_LAYERS_MSP_ANALOGINMANIPULATOR_H_
