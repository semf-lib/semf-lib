/**
 * @file timer.h
 *
 * @date 31.01.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef INTERFACES_TIMER_H_
#define INTERFACES_TIMER_H_

#include <Core/Signals/signal.h>
#include <Core/error.h>

namespace semf
{
/**
 * @brief Class for using timer hardware.
 */
class Timer
{
public:
	virtual ~Timer() = default;
	/**
	 * @brief Starts the timer.
	 */
	virtual void start() = 0;
	/**
	 * @brief Stops the timer.
	 */
	virtual void stop() = 0;
	/**
	 * @brief Resets the timer.
	 */
	virtual void reset() = 0;
	/** Is emitted on timeout.*/
	Signal<> timeout;
	/** Signal is fired if hardware error is occurred.*/
	Signal<1, Error> error;
};

}  /* namespace semf */

#endif  /* INTERFACES_TIMER_H_ */
