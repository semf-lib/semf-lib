/**
 * @file temperature.cpp
 * @date 12.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <bsp/temperature.h>

namespace bsp
{
void Temperature::setFactor(double factor)
{
	m_factor = factor;
}

void Temperature::setOffset(double offset)
{
	m_offset = offset;
}

double Temperature::convert(uint16_t rawValue) const
{
	return (static_cast<double>(rawValue) - m_offset) * m_factor;
}
}  // namespace bsp
