/**
 * @file stm32f4i2cmaster.h
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4I2CMASTER_H_
#define SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4I2CMASTER_H_

#include <semf/hardwareabstraction/stm32/stm32i2cmaster.h>

#if defined(STM32F4) && defined(HAL_I2C_MODULE_ENABLED)
namespace semf
{
/**
 * @brief Specialization of \c Stm32I2cMaster for STM32F4.
 */
class Stm32F4I2cMaster : public Stm32I2cMaster
{
public:
	/**
	 * @brief Error IDs for this class. Error ID identify a unique error() / onError call (excluding transferring).
	 */
	enum class ErrorCode : uint8_t
	{
		SetFrequencyHardware_FrequencyNotSupported = 0
	};
	using Stm32I2cMaster::Stm32I2cMaster;
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
	 * @copydoc Stm32I2cMaster::setFrequencyHardware();
	 * @throws SetFrequencyHardware_FrequencyNotSupported If frequency is bigger than 400000.
	 */
	void setFrequencyHardware(uint32_t hz) override;

private:
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32F4I2cMaster;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4I2CMASTER_H_ */
