/**
 * @file spi.h
 * @date 31.01.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 * 			  For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_COMMUNICATION_SPI_H_
#define SEMF_COMMUNICATION_SPI_H_

#include <semf/system/gpio.h>
#include <cstddef>

namespace semf
{
/**
 * @brief Interface for SPI specific functionalities, which are not solved in a generic way in \c CommunicationHardware.
 *
 * @see \c CommunicationHardware
 * @see \c SpiMasterHardware
 */
class Spi
{
public:
	/** Data transmission mode (POL (polarity) and PHA (phase) setting).*/
	enum class TransmissionMode : uint8_t
	{
		Mode0,  //!< POL is low, PHA is low
		Mode1,  //!< POL is low, PHA is high
		Mode2,  //!< POL is high, PHA is low
		Mode3   //!< POL is high, PHA is high
	};
	/** Wire mode.*/
	enum class WireMode : uint8_t
	{
		FullDuplexWires,  //!< CLK, MISO and MOSI pins used
		HalfDuplexWires,  //!< CLK, MISO / MOSI pins used (MISO and MOSI pin is shared)
		WriteOnlyWires,   //!< CLK and MOSI pins used (no master receive)
		ReadOnlyWires     //!< CLK and MISO pins used (no master write)
	};

	virtual ~Spi() = default;

	/**
	 * @brief For writing and reading data parallel through the communication hardware
	 *        1) For interrupt or one shot dma mode,
	 *           dataAvailable signal will be fired after successful read/write.
	 *        2) For cyclic dma mode, do not use this function.
	 * @param writeData data array for write data
	 * @param readBuffer data array for read data
	 * @param size data array size
	 */
	virtual void writeRead(const uint8_t writeData[], uint8_t readBuffer[], size_t size) = 0;
	/**
	 * @brief Sets the format for the spi slave device
	 * @param bits number of bits per frame
	 * @param transmission transmission mode (polarity and phase setting)
	 * @param wire wire transmission setting
	 */
	virtual void setFormat(uint8_t bits, TransmissionMode transmission, WireMode wire) = 0;
};
} /* namespace semf */
#endif /* SEMF_COMMUNICATION_SPI_H_ */
