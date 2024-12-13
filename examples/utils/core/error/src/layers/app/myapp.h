/**
 * @file myapp.h
 * @date 18.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef EXAMPLES_UTILS_CORE_ERROR_SRC_LAYERS_APP_MYAPP_H_
#define EXAMPLES_UTILS_CORE_ERROR_SRC_LAYERS_APP_MYAPP_H_

#include <app/component.h>
#include <semf/utils/core/signals/slot.h>

namespace app
{
class MyApp
{
public:
	MyApp();
	MyApp(const MyApp& other) = delete;
	virtual ~MyApp() = default;

	void start();

private:
	void onError(semf::Error error);

	Component m_component;
	SEMF_SLOT(m_onErrorSlot, MyApp, *this, onError, semf::Error);
};
}  // namespace app
#endif  // EXAMPLES_UTILS_CORE_ERROR_SRC_LAYERS_APP_MYAPP_H_
