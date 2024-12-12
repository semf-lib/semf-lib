/**
 * @file tickreceiver.h
 * @date 22.02.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 * 			  For detailed information, read the license file in the project
 * 			  root directory.
 */

#ifndef SEMF_SYSTEM_TICKRECEIVER_H_
#define SEMF_SYSTEM_TICKRECEIVER_H_

#include <semf/utils/core/lists/linkedlist.h>
#include <semf/utils/core/signals/signal.h>

namespace semf
{
/**
 * @brief Interface for all classes receiving ticks like \c DigitalInPolling
 * or \c SoftwareTimer.
 *
 * The tick function must be called e.g. in \c Timer or \c TimeBase class periodically.
 * One period is one tick.
 *
 * Because of \c TickReceiver is a \c LinkedList::Node every \c TickReceiver object can be
 * handled in a \c LinkedList like it is done in \c TimerBase.
 */
class TickReceiver : public LinkedList<TickReceiver>::Node
{
public:
	TickReceiver() = default;
	explicit TickReceiver(const TickReceiver& other) = delete;
	virtual ~TickReceiver() = default;

	/**
	 * @brief Is called by every tick.
	 * Is called periodically e.g. by a \c Timer object.
	 */
	virtual void tick() = 0;
};
} /* namespace semf */
#endif  // SEMF_SYSTEM_TICKRECEIVER_H_
