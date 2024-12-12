/**
 * @file clock.h
 * @date 07.06.2023
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_APP_SYSTEM_CLOCK_H_
#define SEMF_APP_SYSTEM_CLOCK_H_

#include <semf/utils/system/datetime.h>

namespace semf
{
namespace app
{
/**
 * @brief The \c Clock class provides real time clock functionality.
 * It can return a \c DateTime object with actual date and time information from \c Rtc
 * and the \c Rtc can be updated by setting a \c DateTime object to \c Clock.
 *
 * The \c Rtc date and time can be modified by set().
 *
 * @see DateTime
 * @see Date
 * @see Time.
 */
class Clock
{
public:
	Clock() = default;
	explicit Clock(const Clock& other) = delete;
	virtual ~Clock() = default;

	/**
	 * @brief Returns the momentary date time from \c Rtc.
	 * @return \c DateTime object with \c Rtc date and time information.
	 */
	virtual DateTime& now() = 0;
	/**
	 * @brief Sets the \c DateTime information from \c datetimeToSet to the \c Rtc.
	 * @param datetimeToSet date and time source for \c Rtc.
	 */
	virtual void set(DateTime& datetimeToSet) = 0;
	/**
	 * @brief Copies the date time.
	 * @param datetimeToSet Source to copy the \c DateTime information from.
	 * @return system clock object itself.
	 */
	virtual Clock& operator=(const DateTime& datetimeToSet) = 0;
};
} /* namespace app */
} /* namespace semf */
#endif /* SEMF_APP_SYSTEM_CLOCK_H_ */
