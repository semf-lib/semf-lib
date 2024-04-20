/**
 * @file externalinterrupt.h
 * @date 18.07.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 * 			  For detailed information, read the license file in the project
 * 			  root directory.
 */

#ifndef SEMF_SYSTEM_EXTERNALINTERRUPT_H_
#define SEMF_SYSTEM_EXTERNALINTERRUPT_H_

#include <semf/utils/core/error.h>
#include <semf/utils/core/signals/signal.h>

namespace semf
{
/**
 * @brief Interface for using a external interrupt.
 */
class ExternalInterrupt
{
public:
	/**Trigger of the EXternalInterrupt signal, it can be /c RisingEdge, /c FallingEdge or /c RisingAndFallingEdge.*/
	enum Trigger
	{
		None = 0,             //<! EXTI is disabled.
		RisingEdge,           //<! EXTI is triggered by rising edge.
		FallingEdge,          //<! EXTI is triggered by falling edge.
		RisingAndFallingEdge  //<! EXTI is triggered by rising and falling edges.
	};

	virtual ~ExternalInterrupt() = default;

	/**
	 * @brief Sets the trigger condition of an EXTI.
	 * @param trigger Trigger condition to set.
	 */
	virtual void setTrigger(Trigger trigger) = 0;

	/**Is emitted when input changed.*/
	Signal<> changed;
	/**Signal is emitted after a hardware error occurred.*/
	Signal<Error> error;
};
} /* namespace semf */
#endif  // SEMF_SYSTEM_EXTERNALINTERRUPT_H_"
