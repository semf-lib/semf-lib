/**
 * @file msp.cpp
 * @date 12.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <msp.h>

namespace msp
{
Msp::Msp()
: m_manipulator(m_analogInTemperatureSensor, m_analogInHumiditySensor)
{
}
semf::VirtualAnalogIn& Msp::analogInTemperatureSensor()
{
	return m_analogInTemperatureSensor;
}

semf::VirtualAnalogIn& Msp::analogInHumiditySensor()
{
	return m_analogInHumiditySensor;
}

AnalogInManipulator& Msp::manipulator()
{
	return m_manipulator;
}
}  // namespace msp
