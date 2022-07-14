/**
 * @file externalinterrupt.h
 *
 * @date 18.07.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 * 			  For detailed information, read the license file in the project
 * 			  root directory.
 */

#ifndef INTERFACES_EXTERNALINTERRUPT_H_
#define INTERFACES_EXTERNALINTERRUPT_H_

#include <Core/Signals/signal.h>
#include <Core/error.h>

namespace semf
{
/**
 * @brief Class for using a external Interrupt.
 */
class ExternalInterrupt
{
public:
	/**
	 * @brief Trigger of the EXternalInterrupt signal, it can be /c RisingEdge, /c FallingEdge or /c RisingAndFallingEdge.
	 */
	enum Trigger
	{
		None = 0,  //<! EXTI is disabled.
		RisingEdge,  //<! EXTI is triggered by rising edge.
		FallingEdge,  //<! EXTI is triggered by falling edge.
		RisingAndFallingEdge  //<! EXTI is triggered by rising and falling edges.
	};
	virtual ~ExternalInterrupt() = default;

	/** Is emitted when input changed*/
	Signal<> changed;
	/**
	 * @brief Sets the trigger condition of an EXTI.
	 * @param trigger Trigger condition to set.
	 */
	virtual void setTrigger(Trigger trigger) = 0;
	/**
	 * @brief Signal is emitted after a hardware error occurred.
	 */
	Signal<1, Error> error;
};

}  /* namespace semf */
#endif  // INTERFACES_EXTERNALINTERRUPT_H_"
