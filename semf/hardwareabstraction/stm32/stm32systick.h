/**
 * @file stm32systick.h
 * @date 31.01.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32_STM32SYSTICK_H_
#define SEMF_HARDWAREABSTRACTION_STM32_STM32SYSTICK_H_

#include <semf/app/system/timer.h>
#include <semf/hardwareabstraction/stm32/stm32.h>
#include <semf/utils/core/debug.h>

#if defined(STM32)
namespace semf
{
template <typename T>
/**
 * @brief \c Timer implementation for STM32 as single systick..
 * @tparam T Controller like STM32F1.
 */
class Stm32Systick : public app::Timer
{
public:
	/**
	 * @brief Gets the single instance
	 * @return The instance of the systick.
	 */
	static T& instance();
	/**Interrupt service routine for timer elapsed.*/
	static void isr();
	void start() override;
	void stop() override;
	void reset() override;

protected:
	Stm32Systick() = default;
};

template <typename T>
T& Stm32Systick<T>::instance()
{
	static T timer;
	return timer;
}

template <typename T>
void Stm32Systick<T>::isr()
{
	SEMF_SINGLETON_INFO(&instance(), "timeout");
	instance().timeout();
}

template <typename T>
void Stm32Systick<T>::start()
{
	volatile uint32_t* sysTickControl = reinterpret_cast<volatile uint32_t*>(0xE000E010);
	*sysTickControl |= 1;
}

template <typename T>
void Stm32Systick<T>::stop()
{
	volatile uint32_t* sysTickControl = reinterpret_cast<volatile uint32_t*>(0xE000E010);
	*sysTickControl &= 0xFFFFFFFE;
}

template <typename T>
void Stm32Systick<T>::reset()
{
	volatile uint32_t* sysTickCount = reinterpret_cast<volatile uint32_t*>(0xE000E018);
	*sysTickCount = 0;
}

} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32_STM32SYSTICK_H_ */
