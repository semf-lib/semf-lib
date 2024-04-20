/**
 * @file clock.h
 * @date 03.09.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_SYSTEM_CLOCK_H_
#define SEMF_SYSTEM_CLOCK_H_

#include <semf/app/system/clock.h>
#include <semf/system/rtc.h>

namespace semf
{
/**
 * @copydoc app::Clock
 */
class Clock : public app::Clock
{
public:
	/**
	 * @brief Constructor.
	 * @param rtc \c Rtc object to use in \c Clock.
	 */
	explicit Clock(Rtc& rtc);
	explicit Clock(const Clock& other) = delete;
	//! @cond Doxygen_Suppress
	virtual ~Clock() = default;
	//! @endcond

	DateTime& now() override;
	void set(DateTime& datetimeToSet) override;
	app::Clock& operator=(const DateTime& datetimeToSet) override;

private:
	/**Pointer to the real time clock*/
	Rtc* m_rtc = nullptr;
	/**Date time object containing rtc information, returned by now.*/
	DateTime m_dateTime;
};
} /* namespace semf */
#endif /* SEMF_SYSTEM_CLOCK_H_ */
