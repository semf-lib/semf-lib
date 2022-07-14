/**
 * @file analogindma.h
 *
 * @date 25.04.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef INTERFACES_ANALOGINDMA_H_
#define INTERFACES_ANALOGINDMA_H_

#include <Core/Buffer/lastinbuffer.h>
#include <Core/Signals/signal.h>
#include <Core/error.h>

namespace semf
{
/**
 * @brief Interface for using ADC (Analog to Digital Conversion) hardware
 * in DMA (Direct Memory Access) mode.
 */
class AnalogInDma
{
public:
	virtual ~AnalogInDma() = default;

	/**
	 * @brief Starts the hardware modules (ADC with DMA) for reading ADC values.
	 * @param buffer Buffer where the read data should be stored.
	 * @param bufferSize Size of buffer in bytes.
	 */
	virtual void start(uint8_t buffer[], size_t bufferSize) = 0;

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

#endif /* INTERFACES_ANALOGINDMA_H_ */
