/**
 * @file myapp.h
 * @date 18.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef EXAMPLES_SYSTEM_TIMEBASE_SRC_LAYERS_APP_MYAPP_H_
#define EXAMPLES_SYSTEM_TIMEBASE_SRC_LAYERS_APP_MYAPP_H_

#include <app/valuex.h>
#include <semf/app/system/timebase.h>
#include <semf/output/analogout.h>
#include <semf/utils/core/signals/slot.h>
#include <semf/utils/system/softwaretimer.h>

namespace app
{
class MyApp
{
public:
	MyApp(semf::app::TimeBase& timeBase100us, semf::app::TimeBase& timeBase1ms, semf::AnalogOut& analogOut);
	MyApp(const MyApp& other) = delete;
	virtual ~MyApp() = default;

private:
	void mainTask();
	void calculationOfA();
	void calculationOfB();

	semf::SoftwareTimer m_timer9s;
	semf::SoftwareTimer m_timer2s;
	semf::SoftwareTimer m_timer5s;
	semf::AnalogOut& m_analogOut;
	ValueX m_a;
	ValueX m_b;
	SEMF_SLOT(m_timer9sSlot, MyApp, *this, mainTask);
	SEMF_SLOT(m_timer2sSlot, MyApp, *this, calculationOfA);
	SEMF_SLOT(m_timer5sSlot, MyApp, *this, calculationOfB);
};
}  // namespace app
#endif  // EXAMPLES_SYSTEM_TIMEBASE_SRC_LAYERS_APP_MYAPP_H_
