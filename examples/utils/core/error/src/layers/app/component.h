/**
 * @file component.h
 * @date 29.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef EXAMPLES_UTILS_CORE_ERROR_SRC_LAYERS_APP_COMPONENT_H_
#define EXAMPLES_UTILS_CORE_ERROR_SRC_LAYERS_APP_COMPONENT_H_

#include <utils/userclasses.h>
#include <cstdint>

namespace app
{
class Component
{
public:
	enum class ErrorCode : uint8_t
	{
		SetBuffer_IsNullptr,
		SetBuffer_SizeIsZero
	};

	void setBuffer(uint8_t buffer[], size_t bufferSize);
	semf::Signal<semf::Error> error;

private:
	uint8_t* m_buffer = 0;
	size_t m_bufferSize = 0;
	static constexpr semf::Error::ClassID kClassId = static_cast<semf::Error::ClassID>(utils::UserClasses::Component0);
};
}  // namespace app
#endif  // EXAMPLES_UTILS_CORE_ERROR_SRC_LAYERS_APP_COMPONENT_H_
