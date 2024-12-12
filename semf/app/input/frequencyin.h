/**
 * @file frequencyin.h
 * @date 06.06.2023
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_APP_INPUT_FREQUENCYIN_H_
#define SEMF_APP_INPUT_FREQUENCYIN_H_

#include <semf/input/inputcapture.h>
#include <semf/utils/core/signals/signal.h>
#include <cstdint>

namespace semf
{
namespace app
{
/**
 * @brief Class for measuring the frequency on a capture input.
 */
class FrequencyIn
{
public:
	FrequencyIn() = default;
	explicit FrequencyIn(const FrequencyIn& other) = delete;
	virtual ~FrequencyIn() = default;

	/**Start reading the input frequency.*/
	virtual void start() = 0;
	/**Stop reading the input frequency.*/
	virtual void stop() = 0;
	/**Reset the last measured period (counter).*/
	virtual void reset() = 0;
	/**
	 * @brief Returns the measured ticks of a signal period
	 * (directly after capturing it and emitted \c dataAvailable signal).
	 * @return The measured ticks of a signal period.
	 */
	virtual uint32_t periodTicks() const = 0;
	/**
	 * @brief Returns the measured time in milliseconds of 2 successive captured signals.
	 * @warning For using this method, the ticks per second must set correctly.
	 * @return The measured time in milliseconds.
	 * @see setTickPerSecond()
	 */
	virtual uint32_t periodInMs() const = 0;
	/**
	 * @brief Returns the measured frequency.
	 * @warning For this method, the ticks per second must be set correctly.
	 * @return The measured frequency.
	 * @see setTickPerSecond()
	 */
	virtual uint32_t frequency() const = 0;

	/**Signal is emitted after a new frequency information is available.*/
	Signal<> dataAvailable;
};
} /* namespace app */
} /* namespace semf */
#endif /* SEMF_APP_INPUT_FREQUENCYIN_H_ */
