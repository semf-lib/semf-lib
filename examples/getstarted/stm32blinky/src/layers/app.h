/**
 * @file app.h
 *
 * @date 09.07.2024
 * @author mm
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef EXAMPLES_GETSTARTED_STM32BLINKY_SRC_LAYERS_APP_H_
#define EXAMPLES_GETSTARTED_STM32BLINKY_SRC_LAYERS_APP_H_

#include <layers/bsp.h>

#include <layers/app/applogic.h>

namespace app
{
class App
{
public:
	explicit App(bsp::Bsp& bsp);
	App(const App& other) = delete;
	virtual ~App() = default;

private:
	AppLogic m_appLogic;
};
}  // namespace app
#endif  // EXAMPLES_GETSTARTED_STM32BLINKY_SRC_LAYERS_APP_H_
