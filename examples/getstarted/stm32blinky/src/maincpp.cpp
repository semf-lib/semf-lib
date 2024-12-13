/**
 * @file main.cpp
 *
 * @date 09.07.2024
 * @author mm
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */
#include <maincpp.h>

#include <layers/msp.h>
#include <layers/bsp.h>
#include <layers/app.h>

void maincpp()
{
	// Create layer architecture
	static msp::Msp msp;
	static bsp::Bsp bsp(msp);
	static app::App app(bsp);

	while (1)
	{
		// do something
	}
}
