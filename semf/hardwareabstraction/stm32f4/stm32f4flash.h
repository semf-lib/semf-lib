/**
 * @file stm32f4flash.h
 * @date 04.06.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4FLASH_H_
#define SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4FLASH_H_

#include <semf/hardwareabstraction/stm32/stm32flash.h>

#if defined(STM32F4) && defined(HAL_FLASH_MODULE_ENABLED)
namespace semf
{
/**
 * @brief Specialization of \c Stm32ExternalInterrupt for STM32F4.
 * @attention This class is currently only for devices till 1MByte Flash-Memory.
 */
class Stm32F4Flash : public Stm32Flash
{
public:
	using Stm32Flash::Stm32Flash;

	size_t sector(uint32_t address) const override;
	uint32_t address(size_t sector) const override;
	size_t sectorSize(size_t sector) const override;
	size_t numberOfSectors() const override;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32F4_STM32F4FLASH_H_ */
