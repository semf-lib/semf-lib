/**
 * @file msp.h
 * @date 12.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef EXAMPLES_INPUT_SENSOR_SRC_LAYERS_MSP_H_
#define EXAMPLES_INPUT_SENSOR_SRC_LAYERS_MSP_H_

#include <semf/hardwareabstraction/virtual/virtualanalogin.h>
#include <semf/hardwareabstraction/virtual/virtualcriticalsection.h>
#include <msp/analoginmanipulator.h>

namespace msp
{
class Msp
{
public:
	Msp();
	explicit Msp(const Msp& other) = delete;
	virtual ~Msp() = default;

	void loop();
	semf::VirtualAnalogIn& analogInTemperatureSensor();
	semf::VirtualAnalogIn& analogInHumiditySensor();
	AnalogInManipulator& manipulator();

private:
	semf::VirtualCriticalSection m_criticalSection;
	semf::VirtualAnalogIn m_analogInTemperatureSensor;
	semf::VirtualAnalogIn m_analogInHumiditySensor;
	AnalogInManipulator m_manipulator;
};
}  // namespace msp
#endif  // EXAMPLES_INPUT_SENSOR_SRC_LAYERS_MSP_H_
