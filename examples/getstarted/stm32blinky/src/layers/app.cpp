/**
 * @file app.cpp
 *
 * @date 09.07.2024
 * @author mm
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <layers/app.h>

namespace app
{
App::App(bsp::Bsp& bsp)
: m_appLogic(bsp.blinkingLedGreen(), bsp.buttonUser())
{
}
}  // namespace app
