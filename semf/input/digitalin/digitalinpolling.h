/**
 * @file digitalinpolling.h
 * @date 22.03.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_INPUT_DIGITALIN_DIGITALINPOLLING_H_
#define SEMF_INPUT_DIGITALIN_DIGITALINPOLLING_H_

#include <semf/app/system/timebase.h>
#include <semf/input/digitalin/digitalin.h>
#include <semf/system/tickreceiver.h>

namespace semf
{
/**
 * @brief Class for reading a digital input in polling mode.
 *
 * This class will be registered in the \c TimeBase to get updated every \c TimeBase cycle.
 *
 * For reading the actual hardware pin level, call the function \c DigitalIn::state().
 *
 * @see \c app::DigitalIn
 * @see \c TickReceiver
 */
class DigitalInPolling : public DigitalIn, public TickReceiver
{
public:
	/**
	 * @brief Constructor.
	 * @param gpio GPIO interface object for hardware access.
	 * @param inverted \c true sets high level of a pin as low and low level of a pin as high.
	 */
	explicit DigitalInPolling(Gpio& gpio, bool inverted = false);
	/**
	 * @brief Constructor.
	 * @param gpio GPIO interface object for hardware access.
	 * @param timeBase The timebase, registering in this object to update pin level.
	 * @param inverted \c true sets high level of a pin as low and low level of a pin as high.
	 */
	DigitalInPolling(Gpio& gpio, app::TimeBase& timeBase, bool inverted = false);
	explicit DigitalInPolling(const DigitalInPolling& other) = delete;
	virtual ~DigitalInPolling() = default;

	/**Checks the level status of the digital input and considers the inverted setting done in \c DigitalIn.*/
	void tick() override;
	State state() const override;

protected:
	/**
	 * @brief Sets the hardware pin status internally.
	 * @note If \c DigitalIn is set inverted, the state has to be inverted here, tool
	 * @param state Pin status.
	 */
	void setState(State state);

private:
	/**Stores the state of the pin after last time called \c tick function.*/
	State m_state;
};
} /* namespace semf */
#endif /* SEMF_INPUT_DIGITALIN_DIGITALINPOLLING_H_ */
