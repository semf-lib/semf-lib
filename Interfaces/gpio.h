/**
 * @file	gpio.h
 *
 * @date	20.04.2018
 * @author	nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef INTERFACES_GPIO_H_
#define INTERFACES_GPIO_H_

#include <Core/Signals/signal.h>
#include <Core/error.h>
#include <cstdint>

namespace semf
{
/**
 * @brief Interface class for using a GPIO pin of the microcontroller.
 */
class Gpio
{
public:
	/**
	 * @brief Direction of a GPIO. It can be \c INPUT or \c OUTPUT
	 */
	enum Direction
	{
		Input = 0,  //<! Pin is defined as digital input.
		OutputPushpull,  //<! Pin in defined as digital output in pushpull mode.
		OutputOpendrain  //<! Pin is defined as digital output in open drain mode.
	};

	/**
	 * @brief Pullup or pulldown setting of a GPIO.
	 */
	enum PullUpPullDown
	{
		NoPullupPulldown = 0,  //<! No pullup and no pulldown.
		Pullup,   //<! Pullup is enabled, pulldown is disabled.
		Pulldown  //<! Pulldown is enbaled, pullup is disabled.
	};

	virtual ~Gpio() = default;

	/**
	 * @brief Sets the output to a high level.
	 */
	virtual void set() = 0;

	/**
	 * @brief Sets the output to a low level.
	 */
	virtual void reset() = 0;

	/**
	 * @brief Returns the current state of the pin.
	 * @return \c true for high level, \c false for low level.
	 */
	virtual bool state() const = 0;

	/**
	 * @brief Returns the direction setting of a GPIO.
	 * @return \c INPUT or \c OUTPUT.
	 */
	virtual Direction direction() const = 0;

	/**
	 * @brief Sets the direction of a GPIO.
	 * @param direction direction to set.
	 */
	virtual void setDirection(Direction direction) = 0;

	/**
	 * @brief Returns the pullup or pulldown setting of a GPIO.
	 * @return \c NoPullupPulldown, \c Pullup or Pulldown.
	 */
	virtual PullUpPullDown pullUpPullDown() const = 0;

	/**
	 * @brief Sets the pullup or pulldown setting of a GPIO.
	 * @param pullUpPullDown pullup/pulldown for setting up.
	 */
	virtual void setPullUpPullDown(PullUpPullDown pullUpPullDown) = 0;

	/** Signal gets emitted when any error occurres.*/
	Signal<1, Error> error;
};

}  /* namespace semf */

#endif  /* INTERFACES_GPIO_H_ */
