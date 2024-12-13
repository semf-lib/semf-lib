/**
 * @file bsp.h
 * @date 18.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef EXAMPLES_SYSTEM_TIMEBASE_SRC_LAYERS_BSP_H_
#define EXAMPLES_SYSTEM_TIMEBASE_SRC_LAYERS_BSP_H_

#include <semf/system/timebase.h>
#include <msp.h>

namespace bsp
{
class Bsp
{
public:
	explicit Bsp(msp::Msp& msp);
	explicit Bsp(const Bsp& other) = delete;
	virtual ~Bsp() = default;

	semf::TimeBase& timeBase1s();
	semf::TimeBase& timeBase100ms();

private:
	semf::TimeBase m_timeBase1s;
	semf::TimeBase m_timeBase100ms;
};
}  // namespace bsp
#endif  // EXAMPLES_SYSTEM_TIMEBASE_SRC_LAYERS_BSP_H_
