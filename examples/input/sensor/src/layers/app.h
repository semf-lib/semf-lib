/**
 * @file app.h
 * @date 12.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef EXAMPLES_INPUT_SENSOR_SRC_LAYERS_APP_H_
#define EXAMPLES_INPUT_SENSOR_SRC_LAYERS_APP_H_

#include <bsp.h>
#include <app/myapp.h>

namespace app
{
class App
{
public:
	explicit App(bsp::Bsp& bsp);
	explicit App(const App& other) = delete;
	virtual ~App() = default;

private:
	MyApp m_app;
};
}  // namespace app
#endif  // EXAMPLES_INPUT_SENSOR_SRC_LAYERS_APP_H_
