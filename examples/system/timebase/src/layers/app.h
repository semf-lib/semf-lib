/**
 * @file app.h
 * @date 18.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef EXAMPLES_SYSTEM_TIMEBASE_SRC_LAYERS_APP_H_
#define EXAMPLES_SYSTEM_TIMEBASE_SRC_LAYERS_APP_H_

#include <bsp.h>
#include <msp.h>
#include <app/myapp.h>

namespace app
{
class App
{
public:
	App(bsp::Bsp& bsp, msp::Msp& msp);
	explicit App(const App& other) = delete;
	virtual ~App() = default;

private:
	MyApp m_app;
};
}  // namespace app
#endif  // EXAMPLES_SYSTEM_TIMEBASE_SRC_LAYERS_APP_H_
