/**
 * @file	inputcapture.h
 *
 * @date	20.04.2018
 * @author	nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef INTERFACES_INPUTCAPTURE_H_
#define INTERFACES_INPUTCAPTURE_H_

#include <Core/Signals/signal.h>
#include <Core/error.h>
#include <cstdint>
#include <climits>

namespace semf
{
/**
 * @brief Class for utilizing the input capture from the uC.
 */
class InputCapture
{
public:
	/**
	 * @brief Trigger settings of the input capture.
	 */
	enum Trigger : uint8_t
	{
		RisingEdge = 0,
		FallingEdge,
		RisingAndFallingEdge
	};

	virtual ~InputCapture() = default;

	/**
	 * @brief Returns the the number of ticks per second (Hardware timer frequency).
	 * @return Ticks per second.
	 */
	virtual unsigned int ticksPerSecond() const = 0;
	/**
	 * @brief Set the maximum number of ticks at which the tick counter will overflow.
	 * @param maxTicks Number of ticks at which the tick counter will overflow.
	 */
	virtual void setMaxTicks(unsigned int maxTicks) = 0;
	/**
	 * @brief Get the maximum number of ticks from the timer.
	 * @return The maximum number of ticks from the timer.
	 */
	virtual unsigned int maxTicks() const = 0;
	/**
	 * @brief Starts capturing the input signals.
	 */
	virtual void start() = 0;
	/**
	 * @brief Stops capturing the input signals.
	 */
	virtual void stop() = 0;
	/**
	 * @brief Get the latest number of ticks (at the last change of the input).
	 * @return The timer ticks at the last change of the input.
	 */
	virtual unsigned int ticks() = 0;
	/**
	 * @brief Sets the trigger source.
	 * @param trigger Trigger source.
	 */
	virtual void setTrigger(Trigger trigger) = 0;
	/**
	 * @brief Is emitted when a new period has been sampled.
	 */
	Signal<> dataAvailable;
	/**
	 * @brief Gets emitted when an error occurres.
	 */
	Signal<1, Error> error;
};

}  /* namespace semf */

#endif  /* INTERFACES_INPUTCAPTURE_H_ */
