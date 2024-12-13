/**
 * @file valuex.h
 * @date 18.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef EXAMPLES_SYSTEM_TIMEBASE_SRC_LAYERS_APP_VALUEX_H_
#define EXAMPLES_SYSTEM_TIMEBASE_SRC_LAYERS_APP_VALUEX_H_

#include <cstdint>

namespace app
{
class ValueX
{
public:
	uint16_t value() const;
	void setValue(uint16_t value);

private:
	uint16_t m_value = 0;
};
}  // namespace app
#endif  // EXAMPLES_SYSTEM_TIMEBASE_SRC_LAYERS_APP_VALUEX_H_
