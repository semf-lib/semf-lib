/**
 * @file timer.h
 * @date 07.06.2023
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_APP_SYSTEM_TIMER_H_
#define SEMF_APP_SYSTEM_TIMER_H_

#include <semf/utils/core/error.h>
#include <semf/utils/core/signals/signal.h>

namespace semf
{
namespace app
{
/**
 * @brief Class for using timer hardware.
 */
class Timer
{
public:
	virtual ~Timer() = default;

	/**Starts the timer.*/
	virtual void start() = 0;
	/**Stops the timer.*/
	virtual void stop() = 0;
	/**Resets the timer.*/
	virtual void reset() = 0;

	/** Is emitted on timeout.*/
	Signal<> timeout;
	/** Signal is fired if hardware error is occurred.*/
	Signal<Error> error;
};
} /* namespace app */
} /* namespace semf */
#endif  // SEMF_APP_SYSTEM_TIMER_H_
