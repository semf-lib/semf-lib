/**
 * @file app.cpp
 * @date 18.08.2024
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <app.h>

namespace app
{
App::App(bsp::Bsp& bsp, msp::Msp& msp)
: m_app(bsp.timeBase1s(), bsp.timeBase100ms(), msp.analogOut())
{
}
}  // namespace app
