/**
 * @file component.cpp
 * @date 29.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <app/component.h>
#include <semf/utils/core/debug.h>

namespace app
{
void Component::setBuffer(uint8_t buffer[], size_t bufferSize)
{
	if (buffer == nullptr)
	{
		SEMF_ERROR("data is nullptr");
		error(semf::Error(kClassId, static_cast<uint8_t>(ErrorCode::SetBuffer_IsNullptr)));
		return;
	}
	if (bufferSize == 0)
	{
		SEMF_ERROR("dataSize is 0");
		error(semf::Error(kClassId, static_cast<uint8_t>(ErrorCode::SetBuffer_SizeIsZero)));
		return;
	}

	m_buffer = buffer;
	m_bufferSize = bufferSize;
}
}  // namespace app
