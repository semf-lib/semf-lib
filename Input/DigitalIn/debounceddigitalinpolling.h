/**
 * @file debounceddigitalinpolling.h
 *
 * @date 22.03.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef INPUT_DIGITALIN_DEBOUNCEDDIGITALINPOLLING_H_
#define INPUT_DIGITALIN_DEBOUNCEDDIGITALINPOLLING_H_

#include <Input/DigitalIn/digitalinpolling.h>
#include <cstdint>

namespace semf
{
/** 
 * @brief Class for reading and debouncing a digital input (e.g. used for buttons or relays inputs).
 * The two signals \c changedToHigh and \c changedToLow will be emitted after the pin level changed
 * and the debounce time is passed.
 *
 * For reading the actual hardware pin level, call the function \c DigitalIn::state().
 *
 * @see \c DigitalIn
 * @see \c DigitalInPolling
 */
class DebouncedDigitalInPolling : public DigitalInPolling
{
public:
	//! @cond Doxygen_Suppress
	using DigitalInPolling::DigitalInPolling;
	//! @endcond

	/**
	 * @brief Constructor.
	 * @param gpio GPIO interface object for hardware access.
	 * @param debounceLowTime Debounce time taken to change from high to low.
	 * @param debounceHighTime Debounce time taken to change from low to high.
	 * @param inverted \c true sets high level of a pin as low and low level of a pin as high.
	 */
	DebouncedDigitalInPolling(Gpio& gpio, uint32_t debounceLowTime, uint32_t debounceHighTime , bool inverted = false);

	/**
	 * @brief Constructor.
	 * @param gpio GPIO interface object for hardware access.
	 * @param timeBase The timebase for the internal timer.
	 * @param debounceLowTime Debounce time taken to change from high to low.
	 * @param debounceHighTime Debounce time taken to change from low to high.
	 * @param inverted \c true sets high level of a pin as low and low level of a pin as high.
	 */
	DebouncedDigitalInPolling(Gpio& gpio, TimeBase& timeBase, uint32_t debounceLowTime, uint32_t debounceHighTime , bool inverted = false);

	/**
	 * @brief Sets the debounce time for switching from low to high.
	 * @param time The debounce high time.
	 */
	void setDebounceHighTime(uint32_t time);

	/**
	 * @brief Sets the debounce time for switching from high to low.
	 * @param time The debounce low time.
	 */
	void setDebounceLowTime(uint32_t time);

	/**
	 * @brief Checks the present pin state and counts the debounce time.
	 * @note This method must be called periodically using a timer (e.g. SysTick).
	 */
	void tick();

private:
	/** Stores the debounce time taken to change from high to low.*/
	uint32_t m_debounceLowTime;
	/** Stores the debounce time taken to change from low to high.*/
	uint32_t m_debounceHighTime;
	/** Debounce-low-time counter.*/
	uint32_t m_lowTime = 0;
	/** Debounce-high-time counter.*/
	uint32_t m_highTime = 0;
};

} /* namespace semf */

#endif /* INPUT_DIGITALIN_DEBOUNCEDDIGITALINPOLLING_H_ */
