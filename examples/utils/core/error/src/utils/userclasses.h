/**
 * @file userclasses.h
 * @date 29.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef EXAMPLES_UTILS_CORE_ERROR_SRC_UTILS_USERCLASSES_H_
#define EXAMPLES_UTILS_CORE_ERROR_SRC_UTILS_USERCLASSES_H_

#include <semf/utils/core/signals/signal.h>
#include <semf/utils/core/error.h>
#include <cstdio>

namespace utils
{
enum class UserClasses : uint32_t
{
	Begin = semf::Error::ClassID::SectionUserBegin,
	Component0,
	// ... All classes with error signales are listed here
	End = semf::Error::ClassID::SectionUserEnd
};
}  // namespace utils
#endif  // EXAMPLES_UTILS_CORE_ERROR_SRC_UTILS_USERCLASSES_H_
