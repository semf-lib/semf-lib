/**
 * @file msp.h
 * @date 18.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef EXAMPLES_SYSTEM_TIMEBASE_SRC_LAYERS_MSP_H_
#define EXAMPLES_SYSTEM_TIMEBASE_SRC_LAYERS_MSP_H_

#include <semf/hardwareabstraction/virtual/virtualanalogout.h>
#include <semf/hardwareabstraction/virtual/virtualcriticalsection.h>
#include <semf/hardwareabstraction/virtual/virtualtimer.h>

namespace msp
{
class Msp
{
public:
	Msp();
	explicit Msp(const Msp& other) = delete;
	virtual ~Msp() = default;

	void init();
	void deinit();
	semf::VirtualTimer& timer1s();
	semf::VirtualTimer& systick100ms();
	semf::VirtualAnalogOut& analogOut();

private:
	semf::VirtualTimer m_timer1s;
	semf::VirtualTimer m_timer100ms;
	semf::VirtualAnalogOut m_analogOut;
	semf::VirtualCriticalSection m_criticalSection;
};
}  // namespace msp
#endif  // EXAMPLES_SYSTEM_TIMEBASE_SRC_LAYERS_MSP_H_
