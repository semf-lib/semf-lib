/**
 * @file analogin.h
 *
 * @date 11.07.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef INTERFACES_ANALOGIN_H_
#define INTERFACES_ANALOGIN_H_

#include <Core/Signals/signal.h>
#include <Core/error.h>
#include <cstdint>

namespace semf
{
/**
 * @brief Interface for using ADC (Analog to Digital Conversion) hardware in interrupt mode.
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

	/**
	 * @brief Stops the ADC hardware.
	 */
	virtual void stop() = 0;

	/**
	 @brief Signal is emitted after \c readData function is executed.
	 */
	Signal<> dataAvailable;

	/**
	 * @brief Signal is emitted after an hardware error occurred.
	 */
	Signal<1, Error> error;
};

} /* namespace semf */

#endif /* INTERFACES_ANALOGIN_H_ */
