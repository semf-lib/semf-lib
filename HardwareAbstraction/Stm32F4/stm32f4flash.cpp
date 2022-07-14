/**
 * @file stm32f4flash.cpp
 *
 * @date 04.06.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <HardwareAbstraction/Stm32F4/stm32f4flash.h>
#include <Core/debug.h>

#if defined(STM32F4)

#include <stm32f4xx_hal_def.h>
#define ADDR_SECTOR_0 ((uint32_t)0x08000000)
#define ADDR_SECTOR_1 ((uint32_t)0x08004000)
#define ADDR_SECTOR_2 ((uint32_t)0x08008000)
#define ADDR_SECTOR_3 ((uint32_t)0x0800C000)
#define ADDR_SECTOR_4 ((uint32_t)0x08010000)
#define ADDR_SECTOR_5 ((uint32_t)0x08020000)
#define ADDR_SECTOR_6 ((uint32_t)0x08040000)
#define ADDR_SECTOR_7 ((uint32_t)0x08060000)
#define ADDR_SECTOR_8 ((uint32_t)0x08080000)
#define ADDR_SECTOR_9 ((uint32_t)0x080A0000)
#define ADDR_SECTOR_10 ((uint32_t)0x080C0000)
#define ADDR_SECTOR_11 ((uint32_t)0x080E0000)
#define ADDR_SECTOR_12 ((uint32_t)0x08100000)
#define ADDR_SECTOR_13 ((uint32_t)0x08104000)
#define ADDR_SECTOR_14 ((uint32_t)0x08108000)
#define ADDR_SECTOR_15 ((uint32_t)0x0810C000)
#define ADDR_SECTOR_16 ((uint32_t)0x08110000)
#define ADDR_SECTOR_17 ((uint32_t)0x08120000)
#define ADDR_SECTOR_18 ((uint32_t)0x08140000)
#define ADDR_SECTOR_19 ((uint32_t)0x08160000)
#define ADDR_SECTOR_20 ((uint32_t)0x08180000)
#define ADDR_SECTOR_21 ((uint32_t)0x081A0000)
#define ADDR_SECTOR_22 ((uint32_t)0x081C0000)
#define ADDR_SECTOR_23 ((uint32_t)0x081E0000)
#define ADDR_SECTOR_END ((uint32_t)0x08200000)

namespace semf
{
size_t Stm32F4Flash::sector(uint32_t address) const
{
	if ((address >= ADDR_SECTOR_8 && size() <= 512) ||
			(address >= ADDR_SECTOR_12 && size() <= 1024) ||
			(address >= ADDR_SECTOR_END))
	{
		SEMF_ERROR("invalid address %u", address);
		return 0;
	}
	else if (address < ADDR_SECTOR_6)
	{
		if (address < ADDR_SECTOR_3)
		{
			if (address < ADDR_SECTOR_1)
				return 0;
			else if (address < ADDR_SECTOR_2)
				return 1;
			else
				return 2;
		}
		else
		{
			if (address < ADDR_SECTOR_4)
				return 3;
			else if (address < ADDR_SECTOR_5)
				return 4;
			else
				return 5;
		}
	}
	else if (address < ADDR_SECTOR_12)
	{
		if (address < ADDR_SECTOR_9)
		{
			if (address < ADDR_SECTOR_7)
				return 6;
			else if (address < ADDR_SECTOR_8)
				return 7;
			else
				return 8;
		}
		else
		{
			if (address < ADDR_SECTOR_10)
				return 9;
			else if (address < ADDR_SECTOR_11)
				return 10;
			else
				return 11;
		}
	}
	else if (address < ADDR_SECTOR_18)
	{
		if (address < ADDR_SECTOR_15)
		{
			if (address < ADDR_SECTOR_13)
				return 12;
			else if (address < ADDR_SECTOR_14)
				return 13;
			else
				return 14;
		}
		else
		{
			if (address < ADDR_SECTOR_16)
				return 15;
			else if (address < ADDR_SECTOR_17)
				return 16;
			else
				return 17;
		}
	}
	else
	{
		if (address < ADDR_SECTOR_21)
		{
			if (address < ADDR_SECTOR_19)
				return 18;
			else if (address < ADDR_SECTOR_20)
				return 19;
			else
				return 20;
		}
		else
		{
			if (address < ADDR_SECTOR_22)
				return 21;
			else if (address < ADDR_SECTOR_23)
				return 22;
			else
				return 23;
		}
	}
}

uint32_t Stm32F4Flash::address(size_t sector) const
{
	switch(sector)
	{
	case 0: return ADDR_SECTOR_0;
	case 1: return ADDR_SECTOR_1;
	case 2: return ADDR_SECTOR_2;
	case 3: return ADDR_SECTOR_3;
	case 4: return ADDR_SECTOR_4;
	case 5: return ADDR_SECTOR_5;
	case 6: return ADDR_SECTOR_6;
	case 7: return ADDR_SECTOR_7;
	case 8: return ADDR_SECTOR_8;
	case 9: return ADDR_SECTOR_9;
	case 10: return ADDR_SECTOR_10;
	case 11: return ADDR_SECTOR_11;
	case 12: return ADDR_SECTOR_12;
	case 13: return ADDR_SECTOR_13;
	case 14: return ADDR_SECTOR_14;
	case 15: return ADDR_SECTOR_15;
	case 16: return ADDR_SECTOR_16;
	case 17: return ADDR_SECTOR_17;
	case 18: return ADDR_SECTOR_18;
	case 19: return ADDR_SECTOR_19;
	case 20: return ADDR_SECTOR_20;
	case 21: return ADDR_SECTOR_21;
	case 22: return ADDR_SECTOR_22;
	case 23: return ADDR_SECTOR_23;
	default:
		SEMF_ERROR("invalid sector %u", sector);
		return 0;
	}
}

size_t Stm32F4Flash::sectorSize(size_t sector) const
{
	if (sector <= 3)
		return 16 * 1024;
	else if (sector == 4)
		return 64 * 1024;
	else if (sector <= 11)
		return 128 * 1024;
	else if (sector <= 15)
		return 16 * 1024;
	else if (sector == 16)
		return 64 * 1024;
	else if (sector <= 23)
		return 128 * 1024;
	else
	{
		SEMF_ERROR("invalid sector %u", sector);
		return 0;
	}
}

size_t Stm32F4Flash::numberOfSectors() const
{
	switch (size())
	{
	case 512:
		return 8;
	case 1024:
		return 12;
	case 2048:
		return 24;
	default:
		return 0;
	}
}

} /* namespace semf */

#endif
