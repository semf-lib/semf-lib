/**
 * @file stm32flash.h
 * @date 04.06.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32_STM32FLASH_H_
#define SEMF_HARDWAREABSTRACTION_STM32_STM32FLASH_H_

#include <semf/hardwareabstraction/stm32/stm32.h>

#if defined(STM32) && defined(HAL_FLASH_MODULE_ENABLED)
#include <semf/app/storage/flash.h>
namespace semf
{
/**
 * @brief This Class is for read and write data in the internal flash from the Stm32
 * @attention To use this class, the flash interrupt must be activated.
 * In addition, the semfStm32FlashIsr() function must be called in the FLASH_IRQHandler() function after
 * HAL_FLASH_IRQHandler() has been called.
 */
class Stm32Flash : public app::Flash
{
public:
	/**Error codes for this class. Error ID identify a unique error() / onError call (excluding transferring).*/
	enum class ErrorCode : uint8_t
	{
		Write_IsBusy = 0,
		Write_DataIsNullptr,
		Write_DataSizeIsZero,
		Write_DataNot8ByteAligned,
		Write_DataNot4ByteAligned,
		Write_DataNot2ByteAligned,
		Write_HalError,
		Write_HalBusy,
		Write_HalTimeout,
		Read_IsBusy = 0,
		Read_BufferIsNullptr,
		Read_BufferSizeIsZero,
		Erase_IsBusy,
		Erase_SectorOutOfBounds,
		Erase_IsHalError,
		Erase_IsHalBusy,
		Erase_IsHalTimeout,
		Isr_HalError,
		Isr_HalBusy,
		Isr_HalTimeout,
		IsrError_InterruptError
	};

	/**
	 * @brief Constructor.
	 * @param size Flash size in kBytes.
	 * @param voltageRange Operating voltage range. See microcontroller user manual
	 * for HAL and LL drivers in flash extension driver section.
	 * @param bank Flash bank which is used.
	 */
	Stm32Flash(uint16_t size, uint32_t voltageRange, uint32_t bank);
	/**
	 * @brief Constructor.
	 * @param size Flash size in kBytes.
	 * @param bank Flash bank which is used.
	 */
	Stm32Flash(uint16_t size, uint32_t bank);
	/**
	 * @brief Constructor.
	 * @param size Flash size in kBytes.
	 */
	explicit Stm32Flash(uint16_t size);
	explicit Stm32Flash(const Stm32Flash& other) = delete;
	virtual ~Stm32Flash() = default;

	/**
	 * @copydoc app::Flash::write()
	 * @throws Write_IsBusy If this is busy.
	 * @throws Write_DataIsNullptr If data is a nullptr.
	 * @throws Write_DataSizeIsZero If dataSize is zero.
	 * @throws Write_DataNot4ByteAligned If the dataSize is not a multiple of 4 (only STM32L0)
	 * @throws Write_DataNot2byteAligned If the dataSize is not a multiple of 2 (only STM32F1 & STM32F3)
	 * @throws Write_HalError If the ST-HAL returns hal error.
	 * @throws Write_HalBusy If the ST-HAL returns hal busy.
	 * @throws Write_HalTimeout If the ST-HAL returns hal timeout.
	 */
	void write(uint32_t address, const uint8_t data[], size_t dataSize) override;
	/**
	 * @copydoc app::Flash::read()
	 * @throws Read_IsBusy If this is busy.
	 * @throws Read_BufferIsNullptr If buffer is a nullptr
	 * @throws Read_BufferSizeIsZero If bufferSize is zero.
	 */
	void read(uint32_t address, uint8_t buffer[], size_t bufferSize) override;
	/**
	 * @copydoc app::Flash::erase()
	 * @throws Erase_IsBusy If this is busy
	 * @throws Erase_SectorOutOfBounds If sector does not exist
	 * @throws Erase_IsHalError If the ST-HAL returns hal error.
	 * @throws Erase_IsHalBusy If the ST-HAL returns hal busy.
	 * @throws Erase_IsHalTimeout If the ST-HAL returns hal timeout.
	 */
	void erase(size_t sector, size_t numOfSectors = 1) override;
	bool isBusy() const override;
	/**
	 * @brief This function must be called from isr.
	 * @throws Isr_HalError If the ST-HAL returns hal error.
	 * @throws Isr_HalBusy If the ST-HAL returns hal busy.
	 * @throws Isr_HalTimeout If the ST-HAL returns hal timeout.
	 */
	static void isr();
	/**
	 * @brief This function is call from isr when an error has occurred.
	 * @throws IsrError_InterruptError If a unexpected error occurres.
	 */
	static void isrError();

protected:
	/**
	 * @brief Returns the size of the flash in kBytes
	 * @return Flash size.
	 */
	uint16_t size() const;

private:
	void write();

	static Stm32Flash* m_flash;
	const uint32_t m_voltageRange;
	const uint16_t m_size;
	const uint32_t m_bank;
	bool m_isBusy = false;
	bool m_eraseIsRunning = false;
	uint32_t m_numOfSecotrsToErase = 0;
	size_t m_bytesToWrite = 0;
	uint8_t* m_dataWrite = nullptr;
	size_t m_addressWrite = 0;
	uint32_t m_addressRead = 0;
	uint8_t* m_dataRead = nullptr;
	size_t m_sizeRead = 0;
	bool m_readIsPending = false;
	bool m_dataAvailablePending = false;
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32Flash;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32_STM32FLASH_H_ */
