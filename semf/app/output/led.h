/**
 * @file led.h
 * @date 06.06.2023
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_APP_OUTPUT_LED_H_
#define SEMF_APP_OUTPUT_LED_H_

#include <cstdint>

namespace semf
{
namespace app
{
/**
 * @brief Interface for all Led classes.
 */
class Led
{
public:
	Led() = default;
	explicit Led(const Led& other) = delete;
	virtual ~Led() = default;

	/**Switch the LED on.*/
	virtual void setOn() = 0;
	/**Switch the LED off.*/
	virtual void setOff() = 0;
	/**Toggle the led state.*/
	virtual void toggle() = 0;
};
} /* namespace app */
} /* namespace semf */
#endif /* SEMF_APP_OUTPUT_LED_H_ */
