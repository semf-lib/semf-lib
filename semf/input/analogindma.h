/**
 * @file analogindma.h
 * @date 25.04.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_INPUT_ANALOGINDMA_H_
#define SEMF_INPUT_ANALOGINDMA_H_

#include <semf/utils/core/error.h>
#include <semf/utils/core/signals/signal.h>

namespace semf
{
/**
 * @brief Interface for using ADC (Analog to Digital Conversion) hardware in DMA (Direct Memory Access) mode.
 *
 * Inheriting class handles a hardware unit with one or more channels.
 *
 * \c dataAvailble signal is emitted after all channels are converted.
 */
class AnalogInDma
{
public:
	AnalogInDma() = default;
	explicit AnalogInDma(const AnalogInDma& other) = delete;
	virtual ~AnalogInDma() = default;

	/**
	 * @brief Starts the hardware modules (ADC with DMA) for reading ADC values.
	 * @param buffer Buffer where the read data should be stored.
	 * @param bufferSize Size of buffer in bytes.
	 */
	virtual void start(uint8_t buffer[], size_t bufferSize) = 0;
	/**Stops the ADC hardware.*/
	virtual void stop() = 0;

	/**Signal is emitted after \c readData function is executed.*/
	Signal<> dataAvailable;
	/**Signal is emitted after an hardware error occurred.*/
	Signal<Error> error;
};
} /* namespace semf */
#endif /* SEMF_INPUT_ANALOGINDMA_H_ */
