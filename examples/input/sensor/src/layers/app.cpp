/**
 * @file app.cpp
 * @date 12.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <app.h>

namespace app
{
App::App(bsp::Bsp& bsp)
: m_app(bsp.temperature(), bsp.humidity())
{
}
}  // namespace app
