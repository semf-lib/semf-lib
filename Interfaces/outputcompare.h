/**
 * @file	outputcompare.h
 *
 * @date	12.03.2020
 * @author	aa
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef INTERFACES_OUTPUTCOMPARE_H_
#define INTERFACES_OUTPUTCOMPARE_H_

#include <Core/Signals/signal.h>
#include <Core/error.h>
#include <Interfaces/timer.h>
#include <cstdint>

namespace semf
{
/**
 * @brief Class to interface \c OutputCompare hardware module of
 * the microcontroller. It compares the value of a selected time base with the value of the
 * compare register, and based on it can generate a \c timeout signal, and/or generates an output pulse.
 */
class OutputCompare : public Timer
{
public:
	virtual ~OutputCompare() = default;

	/**
	 * @brief Sets a output frequency.
	 * @param frequency The new frequency value [hz].
	 */
	void setFrequency(uint32_t frequency /*hz*/);
	/**
	 * @brief Starts the \c OutputCompare hardware module.
	 */
	void start();
	/**
	 * @brief Stops the \c OutputCompare hardware module.
	 */
	void stop();
	/**
	 * @brief Get the output frequency.
	 * @return The output frequency.
	 */
	uint32_t frequency() const;
	/**
	 * @brief Get the current status of \c OutputCompare module.
	 * @return \li true: \c OutputCompare is active,
	 * \li false: \c OutputCompare is not active.
	 */
	bool isEnabled() const;
	/** Signal is fired if hardware error is occurred.*/
	Signal<1, Error> error;

protected:
	/** @brief Initializes Hardware.*/
	virtual void initHardware() = 0;
	/** @brief Starts Hardware.*/
	virtual void startHardware() = 0;
	/** @brief Stops Hardware.*/
	virtual void stopHardware() = 0;
	/** @brief Configures hardware timing registers.*/
	virtual void setTimerConfiguration() = 0;
	/**
	 *@brief Returns the timer's base (input) frequency.
	 *@return Base frequency of the used timer before precaling [KHz].
	 */
	virtual uint32_t timerInputFrequency() const = 0;
	/**
	 * @brief It is called after timer's counter value
	 * reaches the compare register value.
	 * \note It triggers \c timeout signal.
	 */
	virtual void isr() = 0;
private:
	/** Stores whether the module is enabled or not.*/
	bool m_enabled = false;
	/** Stores whether the module is initialized or not.*/
	bool m_isInitialized = false;
	/** The value of output frequency [Hz].*/
	uint32_t m_outputFrequency = 0;
};

}  /* namespace semf */

#endif /* INTERFACES_OUTPUTCOMPARE_H_ */
