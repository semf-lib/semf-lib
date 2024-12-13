/**
 * @file valuex.cpp
 * @date 18.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <app/valuex.h>
#include <semf/system/criticalsection.h>

namespace app
{
uint16_t ValueX::value() const
{
	return m_value;
}

void ValueX::setValue(uint16_t value)
{
	// Write access is secured by critical section
	semf::CriticalSection::enter();
	m_value = value;
	semf::CriticalSection::exit();
}
}  // namespace app
