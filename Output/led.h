/**
 * @file led.h
 *
 * @date 30.09.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef OUTPUT_LED_H_
#define OUTPUT_LED_H_

#include <cstdint>

namespace semf
{
/**
 * @brief Interface for all Led classes.
 */
class Led
{
public:
	virtual ~Led() = default;

	/**
	 * @brief Switch the LED on.
	 */
	virtual void setOn() = 0;

	/**
	 * @brief Switch the LED off.
	 */
	virtual void setOff() = 0;

	/**
	 * @brief Toggle the led state.
	 */
	virtual void toggle() = 0;

	/**
	 * @brief Starts blinking the led with the same on and off time.
	 * @param onOffTime On and off time.
	 */
	virtual void setBlinking(unsigned int onOffTime) = 0;

	/**
	 * @brief Starts blinking the led with different on and off time.
	 * @param onTime On time.
	 * @param offTime Off time.
	 */
	virtual void setBlinking(unsigned int onTime, unsigned int offTime) = 0;
};

} /* namespace semf */

#endif /* OUTPUT_LED_H_ */
