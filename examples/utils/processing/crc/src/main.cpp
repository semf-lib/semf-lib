/**
 * @file main.cpp
 * @author fs
 * @date 28.08.2024
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved.
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/utils/processing/crcsoftware.h>
#include <iostream>
#include <array>

int main()
{
	const uint32_t* crcOut = nullptr;
	// buffer for storing some data, has to be filled somewhere in the code
	std::array<uint8_t, 4 * 100> buffer1 = {13};
	// another buffer for storing some data, has to be filled somewhere in the code
	std::array<uint8_t, 4 * 100> buffer2 = {28};
	// create a 'CrcSoftware' object with a specific datatype, polynomial, initial value, and final xor.
	semf::CrcSoftware<uint32_t, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF> crcSoftware1;
	// or create a default/predefined 'CrcSoftware' object for uint32_t datatype
	semf::Crc32Software crcSoftware2;

	// compute the crc of the buffer1 in one step
	crcSoftware1.reset();
	crcOut = reinterpret_cast<const uint32_t*>(crcSoftware1.calculate(buffer1.data(), buffer1.size()));
	std::cout << "crcOut: " << std::to_string(*crcOut) << std::endl;
	// and check if the crc is as expected
	const uint32_t expectedCrc = 155238192;
	if (crcSoftware1.isEqual(reinterpret_cast<const uint8_t*>(&expectedCrc)))
	{
		std::cout << "nice :-)" << std::endl;
	}
	// compute the crc of the buffer2 in multiple steps
	crcSoftware2.reset();
	for (uint8_t i = 0; i < 100; i++)
	{
		// intermediate result of crc
		crcOut = reinterpret_cast<const uint32_t*>(crcSoftware2.accumulate(&buffer2[i * 4], 4));
	}
	std::cout << "crcOut: " << std::to_string(*crcOut) << std::endl;
	return 0;
}
