/**
 * @file stm32gpio.h
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32_STM32GPIO_H_
#define SEMF_HARDWAREABSTRACTION_STM32_STM32GPIO_H_

#include <semf/hardwareabstraction/stm32/stm32.h>

#if defined(STM32)
#include <semf/system/gpio.h>
namespace semf
{
/**
 * @brief \c Gpio implementation for STM32.
 */
class Stm32Gpio : public Gpio
{
public:
	/**
	 * @brief Constructor.
	 * @param port Port of GPIO.
	 * @param pin Pin of GPIO.
	 */
	Stm32Gpio(GPIO_TypeDef* port, uint16_t pin);
	explicit Stm32Gpio(const Stm32Gpio& other) = delete;
	virtual ~Stm32Gpio() = default;

	void set() override;
	void reset() override;
	bool state() const override;
#ifndef STM32F1
	Direction direction() const override;
	void setDirection(Direction direction) override;
	PullUpPullDown pullUpPullDown() const override;
	void setPullUpPullDown(PullUpPullDown pullUpPullDown) override;
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
#endif /* SEMF_HARDWAREABSTRACTION_STM32_STM32GPIO_H_ */
