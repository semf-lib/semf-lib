/**
 * @file digitalinpolling.h
 *
 * @date 22.03.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef INPUT_DIGITALIN_DIGITALINPOLLING_H_
#define INPUT_DIGITALIN_DIGITALINPOLLING_H_

#include <Input/DigitalIn/digitalin.h>
#include <Interfaces/tickreceiver.h>
#include <System/timebase.h>

namespace semf
{
/** 
 * @brief Class for reading a digital input in polling mode.
 *
 * This class will be registered in the \c TimeBase to get updated every \c TimeBase cycle.
 *
 * For reading the actual hardware pin level, call the function \c DigitalIn::state().
 *
 * @see \c DigitalIn
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
	DigitalInPolling(Gpio& gpio, TimeBase& timeBase, bool inverted = false);

	/**
	 * @brief Checks the level status of the digital input and considers
	 * the inverted setting done in \c DigitalIn.
	 */
	void tick();

	State state() const;

protected:
	/**
	 * @brief Sets the hardware pin status internally.
	 * @note If \c DigitalIn is set inverted, the state has to be inverted here, tool
	 * @param state Pin status.
	 */
	void setState(State state);

private:
	/** Stores the state of the pin after last time called \c tick function.*/
	State m_state;
};

} /* namespace semf */

#endif /* INPUT_DIGITALIN_DIGITALINPOLLING_H_ */
