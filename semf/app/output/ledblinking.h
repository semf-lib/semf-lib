/**
 * @file ledblinking.h
 * @date 06.06.2023
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_APP_OUTPUT_LEDBLINKING_H_
#define SEMF_APP_OUTPUT_LEDBLINKING_H_

#include <semf/app/output/led.h>

namespace semf
{
namespace app
{
/**
 * @brief Interface for a blinking led class.
 */
class LedBlinking : public Led
{
public:
	/**Led operation modes.*/
	enum class Mode : uint8_t
	{
		Off = 0,      //<! Led is off.
		On = 1,       //<! Led is on.
		Blinking = 2  //<! Led is blinking.
	};

	virtual ~LedBlinking() = default;

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
	/**
	 * @brief Returns the active mode of the led.
	 * @return Active led mode setting.
	 */
	virtual Mode mode() const = 0;
};
} /* namespace app */
} /* namespace semf */
#endif /* SEMF_APP_OUTPUT_LEDBLINKING_H_ */
