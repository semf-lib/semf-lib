/**
 * @file error.cpp
 * @date 09.02.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/utils/core/error.h>

namespace semf
{
Error::Error(uint32_t classId, uint8_t errorCode)
: m_classId{classId},
  m_errorCode{errorCode}
{
}

bool Error::isHardwareError() const
{
	return m_classId > ClassID::SectionHardwareBegin && m_classId < ClassID::SectionHardwareEnd;
}

uint32_t Error::classId() const
{
	return m_classId;
}

uint8_t Error::errorCode() const
{
	return m_errorCode;
}
}  // namespace semf
