/**
 * @file flash.h
 * @date 07.06.2023
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_APP_STORAGE_FLASH_H_
#define SEMF_APP_STORAGE_FLASH_H_

#include <semf/app/storage/storage.h>
#include <cstddef>

namespace semf
{
namespace app
{
/**
 * @brief Interface for flash storage.
 */
class Flash : public Storage
{
public:
	Flash() = default;
	explicit Flash(const Flash& other) = delete;
	virtual ~Flash() = default;

	/**
	 * @brief Erase sector(s).
	 * @param sector The numerical order of the targeted sector.
	 * @param numOfSectors Amount of sectors to be erased.
	 */
	virtual void erase(size_t sector, size_t numOfSectors = 1) = 0;
	/**
	 * @brief Return in which sector an address is located.
	 * @attention Return 0, if the address is out of available address range.
	 * @param address Address in a Flash memory.
	 * @return Sector number.
	 */
	virtual size_t sector(uint32_t address) const = 0;
	/**
	 * @brief Return on which address a sector starts.
	 * @param sector Sector index in a flash memory.
	 * @return The address, where a sector starts.
	 */
	virtual uint32_t address(size_t sector) const = 0;
	/**
	 * @brief Return the size of a sector in bytes.
	 * @param sector Sector index in a flash memory.
	 * @return Size of a sector in bytes.
	 */
	virtual size_t sectorSize(size_t sector) const = 0;
	/**@brief Return the number/amount of sectors of a flash memory.
	 * @returns Number of sectors.
	 */
	virtual size_t numberOfSectors() const = 0;

	Signal<> erased; /**< The signal is triggered after the pages have been erased completely.*/
};
} /* namespace app */
} /* namespace semf */
#endif /* SEMF_APP_STORAGE_FLASH_H_ */
