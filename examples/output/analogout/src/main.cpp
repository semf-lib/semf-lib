/**
 * @file main.cpp
 * @author fs
 * @date 17.08.2024
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved.
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/utils/core/signals/staticslot.h>
#include <semf/hardwareabstraction/virtual/virtualanalogout.h>
#include <semf/hardwareabstraction/virtual/virtualanalogoutdma.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <array>
#include <algorithm>

int main()
{
	semf::VirtualAnalogOut analogOut;
	semf::VirtualAnalogOutDma analogOutDma(1);
	semf::StaticSlot<> onAnalogOutDmaDataWrittenSlot([]() { std::cout << "AnalogOutDma buffer is written completely." << std::endl; });
	analogOutDma.dataWritten.connect(onAnalogOutDmaDataWrittenSlot);

	analogOut.setValue(100);
	analogOut.start();
	std::cout << "Started AnalogOut:\t " << analogOut.value() << std::endl;
	analogOut.stop();
	std::cout << "Stoped AnalogOut:\t " << analogOut.value() << std::endl;

	std::array<uint16_t, 10> data;
	std::generate(data.begin(), data.end(),
				  []
				  {
					  static uint16_t i = 0;
					  return i += 100;
				  });
	analogOutDma.setData(reinterpret_cast<uint8_t*>(data.data()), sizeof(data));
	analogOutDma.start();
	for (int i = 0; i < 15; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << "AnalogOutDma:\t " << analogOutDma.value() << std::endl;
	}
	return 0;
}
