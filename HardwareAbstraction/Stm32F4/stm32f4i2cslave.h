/**
 * @file stm32f4i2cslave.h
 *
 * @date 20.01.2019
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32F4_STM32F4I2CSLAVE_H_
#define HARDWAREABSTRACTION_STM32F4_STM32F4I2CSLAVE_H_

#include <HardwareAbstraction/Stm32/stm32i2cslave.h>

#if defined(STM32F4) && defined(HAL_I2C_MODULE_ENABLED)
namespace semf
{
class Stm32F4I2cSlave : public Stm32I2cSlave
{
public:
	/**
	 * @brief Error IDs for this class. Error ID identify a unique error() / onError call (excluding transferring).
	 */
	enum class ErrorCode : uint8_t
	{
		SetFrequencyHardware_FrequencyNotSupported = 0
	};
	using Stm32I2cSlave::Stm32I2cSlave;
	/**
	 * @brief Clears the busy flag if set on startup. See errata sheet.
	 * @param sclPort Port of SCL.
	 * @param sclPin Pin of SCL.
	 * @param sdaPort Port of SDA.
	 * @param sdaPin Pin of SDA.
	 * @param alternate Alternate pin function.
	 */
	void clearBusyFlagErratum(GPIO_TypeDef* sclPort, uint16_t sclPin,
			GPIO_TypeDef* sdaPort, uint16_t sdaPin, uint8_t alternate);
protected:
	/**
	 * @copydoc Stm32I2cSlave::setFrequencyHardware();
	 * @throws SetFrequencyHardware_FrequencyNotSupported If frequency is bigger than 400000.
	 */
	void setFrequencyHardware(uint32_t hz);
private:
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32F4I2cSlave;
};

}  // namespace semf

#endif
#endif  // HARDWAREABSTRACTION_STM32F4_STM32F4I2CSLAVE_H_
