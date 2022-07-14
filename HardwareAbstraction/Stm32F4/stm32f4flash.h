/**
 * @file stm32f4flash.h
 *
 * @date 04.06.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32F4_STM32F4FLASH_H_
#define HARDWAREABSTRACTION_STM32F4_STM32F4FLASH_H_

#include <HardwareAbstraction/Stm32/stm32flash.h>

#if defined(STM32F4) && defined(HAL_FLASH_MODULE_ENABLED)
namespace semf
{
/**
 * @brief This Class is for read and write data in the internal flash from the Stm32F4
 * @attention This class is currently only for devices till 1MByte Flash-Memory.
 */
class Stm32F4Flash : public Stm32Flash
{
public:
	using Stm32Flash::Stm32Flash;

	size_t sector(uint32_t address) const;
	uint32_t address(size_t sector) const;
	size_t sectorSize(size_t sector) const;
	size_t numberOfSectors() const;
};
} /* namespace semf */

#endif
#endif /* HARDWAREABSTRACTION_STM32F4_STM32F4FLASH_H_ */
