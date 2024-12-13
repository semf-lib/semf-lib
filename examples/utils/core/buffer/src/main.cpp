/**
 * @file main.cpp
 * @author fs
 * @date 28.08.2024
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved.
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/utils/core/buffer/lastinbuffer.h>
#include <semf/utils/core/buffer/lastindmabuffer.h>
#include <semf/utils/core/buffer/ringbuffer.h>
#include <semf/utils/processing/average.h>
#include <semf/hardwareabstraction/virtual/virtualcriticalsection.h>
#include <iostream>
#include <array>

int main()
{
	// A critical section object is required for every buffer usage to manage the blocking of the data resource.
	semf::VirtualCriticalSection criticalSection;

	// Ringbuffer
	std::array<uint8_t, 9> buffer = {0};
	semf::RingBuffer<uint8_t> ringBuffer(buffer.data(), buffer.size());
	// Fill buffer
	ringBuffer.put(1);
	ringBuffer.put(2);
	ringBuffer.put(3);
	ringBuffer.put(4);
	ringBuffer.put(5);
	std::cout << "1. RingBuffer" << std::endl;
	std::cout << "Average: " << std::to_string(semf::Average<uint8_t>::value(ringBuffer)) << std::endl;
	// Read data
	while (!ringBuffer.isEmpty())
		std::cout << "Put: " << std::to_string(ringBuffer.get()) << std::endl;
	std::cout << std::endl;

	// LastInBuffer
	std::cout << "2. LastInBuffer" << std::endl;
	semf::LastInBuffer<uint8_t> lastInbuffer(buffer.data(), buffer.size());
	// Fill buffer
	for (uint8_t i = 1; i <= 10; i++)
		lastInbuffer.put(i);
	// Read data
	std::cout << "Value at position 0: " << std::to_string(lastInbuffer[0]) << std::endl;
	std::cout << "Value at position 2: " << std::to_string(lastInbuffer[2]) << std::endl;
	std::cout << "Average: " << std::to_string(semf::Average<uint8_t>::value(lastInbuffer)) << std::endl;
	std::cout << std::endl;

	// LastInDmaBuffer
	std::cout << "3. LastInDmaBuffer" << std::endl;
	semf::LastInDmaBuffer<uint8_t> bufferSensor1(buffer.data(), buffer.size());
	semf::LastInDmaBuffer<uint8_t> bufferSensor2(buffer.data(), buffer.size());
	semf::LastInDmaBuffer<uint8_t> bufferSensor3(buffer.data(), buffer.size());
	// Setting for three LastInDmaBuffer in one data array
	bufferSensor1.setStride(3);
	bufferSensor2.setStride(3);
	bufferSensor3.setStride(3);
	// Setting bufferSens1 first element to array index 0,
	bufferSensor1.setPos(0);
	// bufferSens2 first element to array index 1 and
	bufferSensor2.setPos(1);
	// bufferSens3 first element to array index 2
	bufferSensor3.setPos(2);
	// Fill buffer
	bufferSensor1.put(0x01);  // Data input 1
	bufferSensor1.put(0x02);
	bufferSensor1.put(0x03);
	bufferSensor2.put(0x11);  // Data input 2
	bufferSensor2.put(0x12);
	bufferSensor2.put(0x13);
	bufferSensor3.put(0x21);  // Data input 3
	bufferSensor3.put(0x22);
	bufferSensor3.put(0x23);
	// Read data
	std::cout << "Buffer sensor 1 at position 0: " << std::to_string(bufferSensor1[0]) << std::endl;
	std::cout << "Buffer sensor 2 at position 1: " << std::to_string(bufferSensor2[1]) << std::endl;
	std::cout << "Average sensor 3: " << std::to_string(semf::Average<uint8_t>::value(bufferSensor3)) << std::endl;
	return 0;
}
