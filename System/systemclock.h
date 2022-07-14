/**
 * @file systemclock.h
 *
 * @date 03.09.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SYSTEM_SYSTEMCLOCK_H_
#define SYSTEM_SYSTEMCLOCK_H_

#include <System/datetime.h>
#include <Interfaces/rtc.h>

namespace semf
{
/**
 * @brief The \c SystemClock class provides real time clock functionality.
 * It can return a \c DateTime object with actual date and time information from \c Rtc
 * and the \c Rtc can be updated by setting a \c DateTime object to \c Systemclock.
 *
 * The \c Rtc date and time can be modified by set().
 *
 * See also \c DateTime, \c Date and \c Time.
 */
class SystemClock
{
public:
	/**
	 * @brief Constructor.
	 * @param rtc \c Rtc object to use in \c Systemclock.
	 */
	explicit SystemClock(Rtc& rtc);

	/**
	 * @brief Returns the momentary date time from \c Rtc.
	 * @return \c DateTime object with \c Rtc date and time information.
	 */
	DateTime& now();

	/**
	 * @brief Sets the \c DateTime information from copyfrom to the \c Rtc.
	 * @param datetimeToSet date and time source for rtc.
	 */
	void set(DateTime& datetimeToSet);

	/**
	 * @brief Copies the date time.
	 * @param datetimeToSet Source to copy the \c DateTime information from.
	 * @return system clock object itself.
	 */
	SystemClock& operator=(const DateTime& datetimeToSet);

private:
	/**Pointer to the real time clock*/
	Rtc* m_rtc = nullptr;
	/**Date time object containing rtc information, returned by now.*/
	DateTime m_dateTime;
};

} /* namespace semf */

#endif /* SYSTEM_SYSTEMCLOCK_H_ */
