/**
 * @file	analogout.h
 * @date	24.02.2020
 * @author	sga
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef INTERFACES_ANALOGOUT_H_
#define INTERFACES_ANALOGOUT_H_

#include <Core/Signals/signal.h>
#include <Core/error.h>
#include <cstdint>

namespace semf
{
/**
 * @brief Interface for using DAC (Digital to Analog Converter) hardware module in interrupt mode.
 */
class AnalogOut
{
public:
	virtual ~AnalogOut() = default;

	/**
	 * @brief Starts the hardware module (DAC) for writing DAC values to the output.
	 * Call \c setValue before.
	 */
	virtual void start() const = 0;

	/**
	 * @brief Stops the DAC hardware.
	 */
	virtual void stop() const = 0;

	/**
	 * @brief Writes \c value into hardware register. Call \c start to output the value.
	 * @param value Digital to analog value.
	 */
	virtual void setValue(uint32_t value) = 0;

	/**
	 * @brief Signal is emitted after an hardware error occurred.
	 */
	Signal<1, Error> error;
};

}  /* namespace semf */

#endif /* INTERFACES_ANALOGOUT_H_ */
