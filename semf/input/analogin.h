/**
 * @file analogin.h
 * @date 11.07.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_INPUT_ANALOGIN_H_
#define SEMF_INPUT_ANALOGIN_H_

#include <semf/utils/core/error.h>
#include <semf/utils/core/signals/signal.h>
#include <cstdint>

namespace semf
{
/**
 * @brief Interface for using ADC (Analog to Digital Conversion) hardware in interrupt mode.
 *
 * Inheriting class handles a hardware unit with one or more channels.
 *
 * \c dataAvailble signal is emitted after every converted channel.
 */
class AnalogIn
{
public:
	virtual ~AnalogIn() = default;

	/**
	 * @brief Starts the hardware module (ADC) for reading ADC values.
	 * After finishing analog to digital conversion, the \c dataAvailable signal will be emitted.
	 */
	virtual void start() = 0;
	/**
	 * @brief Returns the last read value.
	 * @note This function should be called after receiving the \c dataAvailable signal.
	 * @return analog value
	 */
	virtual uint32_t value() = 0;
	/**Stops the ADC hardware.*/
	virtual void stop() = 0;

	/**Signal is emitted after one channel is converted.*/
	Signal<> dataAvailable;
	/**Signal is emitted after an error occurred.*/
	Signal<Error> error;
};
} /* namespace semf */
#endif /* SEMF_INPUT_ANALOGIN_H_ */
