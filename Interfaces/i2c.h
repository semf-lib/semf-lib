/**
 * @file	i2c.h
 *
 * @date	20.04.2018
 * @author	nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef INTERFACES_I2C_H_
#define INTERFACES_I2C_H_

#include <cstdint>
#include <cstddef>

namespace semf
{
/**
 * @brief Interface for I2C specific functionalities, which are not solved in a generic way in \c CommunicationHardware.
 *
 * @see \c CommunicationHardware
 * @see \c I2cMasterHardware
 */
class I2c
{
public:
	virtual ~I2c() = default;
	/**
	 * @brief Sets the speed (I2C baud rate).
	 * @param hz Frequency in hz.
	 */
	virtual void setFrequency(uint32_t hz) = 0;
	/**
	 *  @brief Returns the I2C slave device address.
	 *  @return Device address.
	 */
	virtual uint8_t address() const = 0;
	/**
	 * @note Sets the address of the i2c. When this function is called by a master it specifies the address of the slave for further I/O operations.
	 *        When this function is called by a slave it sets the main address of its own.
	 * @param address Device address (0x00 - 0x80)
	 */
	virtual void setAddress(uint8_t address) = 0;
};

}  /* namespace semf */

#endif  /* INTERFACES_I2C_H_ */
