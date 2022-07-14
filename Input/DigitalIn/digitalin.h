/**
 * @file digitalin.h
 *
 * @date 22.03.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef INPUT_DIGITALIN_DIGITALIN_H_
#define INPUT_DIGITALIN_DIGITALIN_H_

#include <Core/Signals/signal.h>
#include <Interfaces/gpio.h>

namespace semf
{
/** 
 * @brief Base class for having access to the level state of a digital input.
 *
 * By setting the inverted flag, the pin level will be forwarded inverse.
 * The two signals \c changedToHigh and \c changedToLow will be emitted after the pin level changed.
 * The function \c state returns the actual status of the pin.
 *
 * This class is designed for application layers. Both, polling (\c DigitalInPolling) and interrupt
 * (\c DigitalInInt) driven inputs, inherit from this class. Also \c DebouncedDigitalInPolling inherits
 * from this class.
 *
 * @see \c DigitalInPolling
 * @see \c DigitalInInt
 */
class DigitalIn
{
public:
	/** Enum for the state of the pin.*/
	enum State : bool
	{
		Low = false, /**< Pin is at low state*/
		High = true /**< Pin is at high state*/
	};

	/**
	 * @brief Constructor.
	 * @param gpio GPIO interface object for hardware access.
	 * @param inverted \c true sets high level of a pin as low and low level of a pin as high.
	 */
	explicit DigitalIn(Gpio& gpio, bool inverted = false);

	/**
	 * @brief Destructor
	 */
	virtual ~DigitalIn() = default;

	/**
	 * @brief Sets GPIO interface object for hardware access.
	 * @param gpio GPIO interface object for hardware access.
	 */
	void setGpio(Gpio& gpio);

	/**
	 * @brief Returns of the pin level logic is inverted.
	 * @return \c true for inverse, otherwise \c false.
	 */
	bool isInverted() const;

	/**
	 * @brief Configures the inversion of the input reading.
	 * @param inverted \c true interprets high level of a pin as low and a low level of a pin as high.
	 */
	void setInverted(bool inverted);

	/**
	 * @brief Returns the level status of the hardware pin.
	 * @note If \c DigitalIn is set inverted, state is inverted.
	 * @return \c High for high level and \c Low for low level.
	 */
	virtual State state() const;

	/** Is emitted after input changed to high.*/
	Signal<> changedToHigh;
	/** Is emitted after input change to low.*/
	Signal<> changedToLow;

protected:
	/**
	 * @brief Returns the pointer to the GPIO to check the status from.
	 * @return Pointer to the GPIO.
	 */
	Gpio& gpio() const;

private:
	/** Pointer to the GPIO interface, that accesses the hardware.*/
	Gpio& m_gpio;
	/** enables reading the input as inverted.*/
	bool m_inverted;
};

} /* namespace semf */

#endif /* INPUT_DIGITALIN_DIGITALIN_H_ */
