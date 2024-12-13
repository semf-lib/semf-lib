/**
 * @file myapp.cpp
 * @date 18.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <app/myapp.h>
#include <semf/utils/core/debug.h>
#include <iostream>

namespace app
{
MyApp::MyApp()
{
	m_component.error.connect(m_onErrorSlot);
	SEMF_DEBUG_ADD_CLASS(Component, semf::DebugLevel::DebuglevelInfo);
}

void MyApp::start()
{
	m_component.setBuffer(nullptr, 100);
}
void MyApp::onError(semf::Error error)
{
	std::cout << "Error: class id: " << std::to_string(error.classId()) << "; error code: " << std::to_string(error.errorCode()) << std::endl;
}
}  // namespace app
