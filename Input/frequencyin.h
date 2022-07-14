/**
 * @file frequencyin.h
 *
 * @date 15.03.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef INPUT_FREQUENCYIN_H_
#define INPUT_FREQUENCYIN_H_

#include <Core/Signals/signal.h>
#include <Interfaces/inputcapture.h>
#include <cstdint>

namespace semf
{
/**
 * @brief Class for measuring the frequency on a capture input.
 */
class FrequencyIn
{
public:
	/**
	 * @brief Constructor.
	 * @param inputCapture Reference to a hardware interface.
	 */
	explicit FrequencyIn(InputCapture& inputCapture);

	virtual ~FrequencyIn() = default;

	/**
	 * @brief Sets the interface to the class for hardware access.
	 * @param inputCapture Reference to a hardware interface.
	 */
	void setInputCapture(InputCapture& inputCapture);

	/**
	 * @brief Start reading the input frequency.
	 */
	void start();

	/**
	 * @brief Stop reading the input frequency.
	 */
	void stop();

	/**
	 * @brief Reset the last measured period (counter).
	 */
	void reset();

	/**
	 * @brief Returns the measured ticks of a signal period
	 * (directly after capturing it and emitted \c dataAvailable signal).
	 * @return The measured ticks of a signal period.
	 */
	uint32_t periodTicks() const;

	/**
	 * @brief Returns the measured time in milliseconds of 2 successive captured signals.
	 * @warning For using this method, the ticks per second must set correctly.
	 * @return The measured time in milliseconds.
	 * @see setTickPerSecond()
	 */
	uint32_t periodInMs() const;

	/**
	 * @brief Returns the measured frequency.
	 * @warning For this method, the ticks per second must be set correctly.
	 * @return The measured frequency.
	 * @see setTickPerSecond()
	 */
	uint32_t frequency() const;

	/**Signal is emitted after a new frequency information is available.*/
	Signal<> dataAvailable;

protected:
	/**
	 * @brief This method must be called from the interrupt routine to update the frequency.
	 */
	virtual void onDataAvailable();

private:
	InputCapture& m_inputCapture;  //!< Hardware interface.
	uint32_t m_lastTicks = 0;  //!< Tick number of the Last measurement cycle.
	uint32_t m_lastPeriod = 0;  //!< Delta Tick of the Last measurement cycle.
};

} /* namespace semf */

#endif /* INPUT_FREQUENCYIN_H_ */
