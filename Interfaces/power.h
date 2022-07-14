/**
 * @file power.h
 *
 * @date 13.12.2018
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef INTERFACES_POWER_H_
#define INTERFACES_POWER_H_

namespace semf
{
/**
 * @brief Class for handling power states of the microcontroller like sleep mode and software reset.
 */
class Power
{
public:
	virtual ~Power() = default;

	/**
	 * @brief Executes a software reset on the microcontroller.
	 */
	virtual void reset() = 0;
	/**
	 * @brief Switches microcontroller directly into standby mode.
	 * @note Stanby is called deepsleep for some microcontrollers and does need
	 * less power than \c sleep.
	 */
	virtual void standby() = 0;
	/**
	 * @brief Switches microcontroller directly into sleep mode.
	 * @note Sleep is called lightsleep for some microcontrollers and does need
	 * more power than \c standby.
	 */
	virtual void sleep() = 0;
	/**
	 * @brief Switches microcontroller directly into stop mode.
	 * @note Stops the program code, while the microcontroller is still running.
	 * Normally stop mode is exited by an interrupt.
	 */
	virtual void stop() = 0;
};

}  /* namespace semf */

#endif  /* INTERFACES_POWER_H_ */
