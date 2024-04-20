/**
 * @file stm32f4i2cmaster.cpp
 *
 * @date 03.02.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/hardwareabstraction/stm32f4/stm32f4i2cmaster.h>
#include <semf/utils/core/debug.h>

#if defined(STM32F4) && defined(HAL_I2C_MODULE_ENABLED)
namespace semf
{
void Stm32F4I2cMaster::clearBusyFlagErratum(GPIO_TypeDef* sclPort, uint16_t sclPin,
		GPIO_TypeDef* sdaPort, uint16_t sdaPin, uint8_t alternate)
{
	if (__HAL_I2C_GET_FLAG(hardwareHandle(), I2C_FLAG_BUSY) == RESET)
		return;

	GPIO_InitTypeDef GPIO_InitStructure;

	// 1. Clear PE bit.
	hardwareHandle()->Instance->CR1 &= ~(0x0001);

	//  2. Configure the SCL and SDA I/Os as General Purpose Output Open-Drain, High level (Write 1 to GPIOx_ODR).
	GPIO_InitStructure.Mode         = GPIO_MODE_OUTPUT_OD;
	// GPIO_InitStructure.Alternate    = I2C_PIN_MAP;
	GPIO_InitStructure.Pull         = GPIO_PULLUP;
	GPIO_InitStructure.Speed        = GPIO_SPEED_FREQ_HIGH;

	GPIO_InitStructure.Pin          = sclPin;
	HAL_GPIO_Init(sclPort, &GPIO_InitStructure);
	HAL_GPIO_WritePin(sclPort, sclPin, GPIO_PIN_SET);

	GPIO_InitStructure.Pin          = sdaPin;
	HAL_GPIO_Init(sdaPort, &GPIO_InitStructure);
	HAL_GPIO_WritePin(sdaPort, sdaPin, GPIO_PIN_SET);

	// 3. Check SCL and SDA High level in GPIOx_IDR.
	while (GPIO_PIN_SET != HAL_GPIO_ReadPin(sclPort, sclPin))
	{
	asm("nop");
	}

	while (GPIO_PIN_SET != HAL_GPIO_ReadPin(sdaPort, sdaPin))
	{
	asm("nop");
	}

	// 4. Configure the SDA I/O as General Purpose Output Open-Drain, Low level (Write 0 to GPIOx_ODR).
	HAL_GPIO_WritePin(sdaPort, sdaPin, GPIO_PIN_RESET);

	//  5. Check SDA Low level in GPIOx_IDR.
	while (GPIO_PIN_RESET != HAL_GPIO_ReadPin(sdaPort, sdaPin))
	{
	asm("nop");
	}

	// 6. Configure the SCL I/O as General Purpose Output Open-Drain, Low level (Write 0 to GPIOx_ODR).
	HAL_GPIO_WritePin(sclPort, sclPin, GPIO_PIN_RESET);

	//  7. Check SCL Low level in GPIOx_IDR.
	while (GPIO_PIN_RESET != HAL_GPIO_ReadPin(sclPort, sclPin))
	{
	asm("nop");
	}

	// 8. Configure the SCL I/O as General Purpose Output Open-Drain, High level (Write 1 to GPIOx_ODR).
	HAL_GPIO_WritePin(sclPort, sclPin, GPIO_PIN_SET);

	// 9. Check SCL High level in GPIOx_IDR.
	while (GPIO_PIN_SET != HAL_GPIO_ReadPin(sclPort, sclPin))
	{
	asm("nop");
	}

	// 10. Configure the SDA I/O as General Purpose Output Open-Drain , High level (Write 1 to GPIOx_ODR).
	HAL_GPIO_WritePin(sdaPort, sdaPin, GPIO_PIN_SET);

	// 11. Check SDA High level in GPIOx_IDR.
	while (GPIO_PIN_SET != HAL_GPIO_ReadPin(sdaPort, sdaPin))
	{
	asm("nop");
	}

	// 12. Configure the SCL and SDA I/Os as Alternate function Open-Drain.
	GPIO_InitStructure.Mode         = GPIO_MODE_AF_OD;
	GPIO_InitStructure.Alternate    = alternate;

	GPIO_InitStructure.Pin          = sclPin;
	HAL_GPIO_Init(sclPort, &GPIO_InitStructure);

	GPIO_InitStructure.Pin          = sdaPin;
	HAL_GPIO_Init(sdaPort, &GPIO_InitStructure);

	// 13. Set SWRST bit in I2Cx_CR1 register.
	hardwareHandle()->Instance->CR1 |= 0x8000;

	asm("nop");

	// 14. Clear SWRST bit in I2Cx_CR1 register.
	hardwareHandle()->Instance->CR1 &= ~0x8000;

	asm("nop");

	// 15. Enable the I2C peripheral by setting the PE bit in I2Cx_CR1 register
	hardwareHandle()->Instance->CR1 |= 0x0001;

	// Call initialization function.
	HAL_I2C_Init(hardwareHandle());
}

void Stm32F4I2cMaster::setFrequencyHardware(uint32_t hz)
{
	if (hz > 400000)
	{
		SEMF_ERROR("frequency not supported, max frequency is 400kHz");
		onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetFrequencyHardware_FrequencyNotSupported)));
		return;
	}

	hardwareHandle()->Init.ClockSpeed = hz;
}
} /* namespace semf */
#endif
