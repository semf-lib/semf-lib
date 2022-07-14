/**
 * @file	analogoutdma.h
 * @date	24.03.2020
 * @author	fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef INTERFACES_ANALOGOUTDMA_H_
#define INTERFACES_ANALOGOUTDMA_H_

#include <Core/Signals/signal.h>
#include <Core/error.h>
#include <cstdint>

namespace semf
{
/**
 * @brief Interface for using DAC (Digital to Analog Converter) hardware module
 * in DMA (Direct Memory Access) mode.
 */
class AnalogOutDma
{
public:
	virtual ~AnalogOutDma() = default;

	/**
	 * @brief Starts the hardware module (DAC) for writing DAC values to the output.
	 * @attention Call \c setData() before. Otherwise will call \c error signal.
	 */
	virtual void start() const = 0;

	/**
	 * @brief Stops the DAC hardware.
	 */
	virtual void stop() const = 0;

	/**
	 * @brief Sets the pointer to the data memory location and its size for starting DAC later by \c start() function.
	 * to output them on the \c AnalogOutDma pin.
	 * @note For using halfword DAC setting, use a uint16_t array, cast it to uint8_t* and double the size of the array.
	 * For using word DAC setting, use a uint32_t array, cast it to uint8_t* and multiple the size of the array by four.
	 * @param data Pointer to the digital to analog value array.
	 * @param dataSize Byte size of the value array
	 */
	virtual void setData(const uint8_t data[], size_t dataSize) = 0;

	/**
	 * @brief Signal is emitted after hardware write cycle is finished.
	 */
	Signal<> dataWritten;

	/**
	 * @brief Signal is emitted after an hardware error occurred.
	 */
	Signal<1, Error> error;
};

}  /* namespace semf */

#endif /* INTERFACES_ANALOGOUTDMA_H_ */
