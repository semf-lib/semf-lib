/**
 * @file uart.h
 *
 * @date 25.02.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef INTERFACES_UART_H_
#define INTERFACES_UART_H_

#include <cstdint>
#include <cstddef>

namespace semf
{
/**
 * @brief Interface for UART specific functionalities, which are not solved in a generic way in \c CommunicationHardware.
 *
 * @see \c CommunicationHardware
 * @see \c UartHardware
 */
class Uart
{
public:
	/**
	 * @brief Flow control modes for using RTS / CTS.
	 */
	enum class FlowControl: uint8_t
	{
		NoFlowControl = 0x00,  //!< No flow control setting, only RXD and TXD pins are used.
		RtsFlowControl = 0x01,  //!< Receive flow control setting, RXD, TXD and RTS pins are used.
		CtsFlowControl = 0x10,  //!< Transmit flow control setting, RXD, TXD and CTS pins are used.
		RtsAndCtsFlowControl = 0x11  //!< Full flow control setting, RXD, TXD, CTS and RTS pins are used.
	};

	/**
	 * @brief Parity setting.
	 */
	enum class Parity: uint8_t
	{
		NoParity,
		OddParity,
		EvenParity
	};
	/**
	 * @brief Number of stop bits.
	 */
	enum class StopBits: uint8_t
	{
		Stopbits_0_5,
		Stopbits_1,
		Stopbits_1_5,
		Stopbits_2,
	};
	/**
	 * @brief Wire mode for full- or half duplex mode.
	 */
	enum class WireMode: uint8_t
	{
		ReadWire,
		WriteWire,
		ReadAndWriteWires
	};

	virtual ~Uart() = default;

	/**
	 * @brief Sets the format for the spi slave device.
	 * @param bits Number of data bits (without parity) per frame.
	 * @param wire Full duplex mode or half duplex mode.
	 * @param par Parity.
	 * @param stop Stop bits.
	 * @param flow Flow control.
	 * @note Number of data bits differs that the number of bits to be send.
	 * i.e.: in case of sending a frame 8 bits without parity, number of data bits should be 8 bits.
	 * i.e.: in case of sending a frame of 8 bits including a parity, number of bits should be 7 bits.
	 */
	virtual void setFormat(uint8_t bits, WireMode wire, Parity par, StopBits stop, FlowControl flow) = 0;
	/**
	 * @brief Sets the baud rate.
	 * @param baud Baud rate.
	 */
	virtual void setBaud(uint32_t baud) = 0;
	/**
	 * @brief Returns the currently used baud rate.
	 * @return Baud rate. 
	 */
	virtual uint32_t baud() = 0;
};

}  /* namespace semf */

#endif  /* INTERFACES_UART_H_ */
