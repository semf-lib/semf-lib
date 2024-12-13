/**
 * @file main.cpp
 * @author fs
 * @date 12.08.2024
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved.
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <app.h>
#include <bsp.h>
#include <msp.h>
#include <thread>
#include <chrono>

int main()
{
	msp::Msp msp;
	bsp::Bsp bsp(msp);
	app::App app(bsp, msp);
	msp.init();
	std::this_thread::sleep_for(std::chrono::milliseconds(20000));
	msp.deinit();
	return 0;
}