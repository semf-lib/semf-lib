/**
 * @file bsp.cpp
 * @date 18.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <bsp.h>

namespace bsp
{
Bsp::Bsp(msp::Msp& msp)
: m_timeBase1s(msp.timer1s(), true),
  m_timeBase100ms(msp.systick100ms(), true)
{
}

semf::TimeBase& Bsp::timeBase1s()
{
	return m_timeBase1s;
}
semf::TimeBase& Bsp::timeBase100ms()
{
	return m_timeBase100ms;
}

}  // namespace bsp
