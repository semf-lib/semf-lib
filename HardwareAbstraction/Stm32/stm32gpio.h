/**
 * @file stm32gpio.h
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32_STM32GPIO_H_
#define HARDWAREABSTRACTION_STM32_STM32GPIO_H_

#include <HardwareAbstraction/Stm32/stm32.h>

#if defined(STM32)
#include <Interfaces/gpio.h>
namespace semf
{
class Stm32Gpio : public Gpio
{
public:
	/**
	 * @brief Constructor.
	 * @param port Port of GPIO.
	 * @param pin Pin of GPIO.
	 */
	Stm32Gpio(GPIO_TypeDef* port, uint16_t pin);

	void set();
	void reset();
	bool state() const;
#ifndef STM32F1
	Direction direction() const;
	void setDirection(Direction direction);
	PullUpPullDown pullUpPullDown() const;
	void setPullUpPullDown(PullUpPullDown pullUpPullDown);
#endif

protected:
	/**
	 * @brief Returns the GPIO handler.
	 * @return Pointer to the GPIO handler.
	 */
	GPIO_TypeDef* port() const;
	/**
	 * @brief Returns the GPIO handler.
	 * @return Pointer to the GPIO handler.
	 */
	GPIO_TypeDef* port();
	/**
	 * @brief Returns pin number.
	 * @return Pin number.
	 */
	uint16_t pin() const;
	/**
	 * @brief Returns the io position based on the pin number.
	 * @param pin Number of pin, given by STM HAL.
	 * @return Bit position to set IO registers.
	 */
	uint16_t pinNumberToIOPosition(uint16_t pin) const;

private:
	/**Gpio Port*/
	GPIO_TypeDef* m_port;
	/**Gpio Pin (needed for read and write)*/
	uint16_t m_pin;
};

} /* namespace semf */

#endif

#endif /* HARDWAREABSTRACTION_STM32_STM32GPIO_H_ */
