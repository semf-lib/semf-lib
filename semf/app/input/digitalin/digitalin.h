/**
 * @file digitalin.h
 * @date 06.06.2023
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_APP_INPUT_DIGITALIN_DIGITALIN_H_
#define SEMF_APP_INPUT_DIGITALIN_DIGITALIN_H_

#include <semf/utils/core/signals/signal.h>

namespace semf
{
namespace app
{
/**
 * @brief Interfaces for having access to the level state of a digital input.
 *
 * By setting the inverted flag, the pin level will be forwarded inverse.
 * The two signals \c changedToHigh and \c changedToLow will be emitted after the pin level changed.
 * The function \c state returns the actual status of the pin.
 *
 * This class is designed for application layers. Both, polling (\c DigitalInPolling) and interrupt
 * (\c DigitalInInt) driven inputs, inherit from this class. Also \c DebouncedDigitalInPolling inherits
 * from this class.
 *
 * @see \c DebouncedDigitalInPolling
 * @see \c DigitalInPolling
 * @see \c DigitalInInt
 */
class DigitalIn
{
public:
	/** Enum for the state of the pin.*/
	enum State : bool
	{
		Low = false, /**< Pin is at low state*/
		High = true  /**< Pin is at high state*/
	};

	virtual ~DigitalIn() = default;

	/**
	 * @brief Returns of the pin level logic is inverted.
	 * @return \c true for inverse, otherwise \c false.
	 */
	virtual bool isInverted() const = 0;
	/**
	 * @brief Configures the inversion of the input reading.
	 * @param inverted \c true interprets high level of a pin as low and a low level of a pin as high.
	 */
	virtual void setInverted(bool inverted) = 0;
	/**
	 * @brief Returns the level status of the hardware pin.
	 * @note If \c DigitalIn is set inverted, state is inverted.
	 * @return \c High for high level and \c Low for low level.
	 */
	virtual State state() const = 0;

	/** Is emitted after input changed to high.*/
	Signal<> changedToHigh;
	/** Is emitted after input change to low.*/
	Signal<> changedToLow;
};
} /* namespace app */
} /* namespace semf */
#endif /* SEMF_APP_INPUT_DIGITALIN_DIGITALIN_H_ */
