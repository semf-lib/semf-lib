/**
 * @file app.h
 * @date 18.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef EXAMPLES_UTILS_CORE_ERROR_SRC_LAYERS_APP_H_
#define EXAMPLES_UTILS_CORE_ERROR_SRC_LAYERS_APP_H_

#include <app/myapp.h>

namespace app
{
class App
{
public:
	App() = default;
	explicit App(const App& other) = delete;
	virtual ~App() = default;

	void start();

private:
	MyApp m_app;
};
}  // namespace app
#endif  // EXAMPLES_UTILS_CORE_ERROR_SRC_LAYERS_APP_H_
