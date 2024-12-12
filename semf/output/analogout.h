/**
 * @file	analogout.h
 * @date	24.02.2020
 * @author	sga
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_OUTPUT_ANALOGOUT_H_
#define SEMF_OUTPUT_ANALOGOUT_H_

#include <semf/utils/core/error.h>
#include <semf/utils/core/signals/signal.h>
#include <cstdint>

namespace semf
{
/**
 * @brief Interface for using DAC (Digital to Analog Converter) hardware module in interrupt mode.
 */
class AnalogOut
{
public:
	AnalogOut() = default;
	explicit AnalogOut(const AnalogOut& other) = delete;
	//! @cond Doxygen_Suppress
	virtual ~AnalogOut() = default;
	//! @endcond

	/**
	 * @brief Starts the hardware module (DAC) for writing DAC values to the output.
	 * Call \c setValue before.
	 */
	virtual void start() = 0;
	/**Stops the DAC hardware.*/
	virtual void stop() = 0;
	/**
	 * @brief Writes \c value into hardware register. Call \c start to output the value.
	 * @param value Digital to analog value.
	 */
	virtual void setValue(uint32_t value) = 0;

	/**Signal is emitted after an hardware error occurred.*/
	Signal<Error> error;
};
} /* namespace semf */
#endif /* SEMF_OUTPUT_ANALOGOUT_H_ */
