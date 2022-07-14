/**
 * @file stm32flash.cpp
 *
 * @date 04.06.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <HardwareAbstraction/Stm32/stm32flash.h>
#include <Core/debug.h>

#if defined(STM32) && defined(HAL_FLASH_MODULE_ENABLED)
extern FLASH_ProcessTypeDef pFlash;

extern "C"
/**
 * @brief This function must be called in the HAL_FLASH_EndOfOperationCallback().
 * @attention This function must be called in in the FLASH_IRQHandler() function after
 * HAL_FLASH_IRQHandler() has been called.
 */
void semfStm32FlashIsr()
{
	semf::Stm32Flash::isr();
}

namespace semf
{
Stm32Flash* Stm32Flash::m_flash;
Stm32Flash::Stm32Flash(uint16_t size, uint32_t voltageRange, bool dualBankMode)
	:m_voltageRange(voltageRange),
	 m_size(size),
	 m_dualBank(dualBankMode)
{
	Stm32Flash::m_flash = this;
}

Stm32Flash::Stm32Flash(uint16_t size, bool dualBankMode)
	:m_voltageRange(0),
	 m_size(size),
	 m_dualBank(dualBankMode)
{
	Stm32Flash::m_flash = this;
}

void Stm32Flash::write(uint32_t address, const uint8_t data[], size_t dataSize)
{
	SEMF_INFO("address %u, data %p, size is %u", address, data, dataSize);
	if (m_isBusy)
	{
		SEMF_ERROR("is busy");
		m_flash->error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Write_IsBusy)));
		return;
	}
	else if (data == nullptr)
	{
		SEMF_ERROR("data is nullptr");
		m_flash->error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Write_DataIsNullptr)));
		return;
	}
	else if (dataSize == 0)
	{
		SEMF_ERROR("size is 0");
		m_flash->error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Write_DataSizeIsZero)));
		return;
	}

	m_isBusy = true;
	m_addressWrite = address;
	m_dataWrite = const_cast<uint8_t*>(data);
	m_bytesToWrite = dataSize;

	HAL_FLASH_Unlock();
	write();
}

void Stm32Flash::read(uint32_t address, uint8_t buffer[], size_t bufferSize)
{
	SEMF_INFO("address %u, data %p, size is %u", address, buffer, bufferSize);
	if (m_isBusy)
	{
		SEMF_ERROR("is busy");
		m_flash->error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Read_IsBusy)));
		return;
	}
	else if (buffer == nullptr)
	{
		SEMF_ERROR("buffer is nullptr");
		m_flash->error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Read_BufferIsNullptr)));
		return;
	}
	else if (bufferSize == 0)
	{
		SEMF_ERROR("bufferSize is 0");
		m_flash->error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Read_BufferSizeIsZero)));
	}

	if (m_dataAvailablePending)
	{
		m_addressRead = address;
		m_dataRead = buffer;
		m_sizeRead = bufferSize;
		m_readIsPending = true;
		return;
	}
	else
	{
		while (1)
		{
			for (size_t i = 0; i < bufferSize; i++)
			{
				buffer[i] = (*(__IO uint8_t*)(address));
				address++;
			}

			m_dataAvailablePending = true;
			m_isBusy = false;
			dataAvailable();
			m_dataAvailablePending = false;

			if (m_readIsPending)
			{
				address = m_addressRead;
				buffer = m_dataRead;
				bufferSize = m_sizeRead;
				m_readIsPending = false;
			}
			else
			{
				break;
			}
		}
	}
}

void Stm32Flash::erase(size_t sector, size_t numOfSecotrs)
{
	SEMF_INFO("first sector index %u, number of sectors %u", sector, numOfSecotrs);
	if (m_isBusy)
	{
		SEMF_ERROR("is busy");
		m_flash->error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Erase_IsBusy)));
		return;
	}
	if (sector >= numberOfSectors())  // bad error handeling
	{
		SEMF_ERROR("sector %u is out of range", sector);
		m_flash->error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Erase_SectorOutOfBounds)));
		return;
	}

	m_isBusy = true;
	m_numOfSecotrsToErase = numOfSecotrs;
	m_eraseIsRunning = true;

	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef eraseinit;
#if defined(STM32F1) || defined(STM32F3) || defined(STM32L0) || defined(STM32F0)
	eraseinit.TypeErase = FLASH_TYPEERASE_PAGES;
	eraseinit.PageAddress = address(sector);
	eraseinit.NbPages = numOfSecotrs;
#else
	eraseinit.TypeErase = FLASH_TYPEERASE_SECTORS;
	eraseinit.Sector = sector;
	eraseinit.NbSectors = numOfSecotrs;
	eraseinit.VoltageRange = m_voltageRange;
#endif
	HAL_StatusTypeDef state = HAL_FLASHEx_Erase_IT(&eraseinit);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			m_flash->error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Erase_IsHalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			m_flash->error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Erase_IsHalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			m_flash->error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Erase_IsHalTimeout)));
		}
		m_isBusy = false;
		return;
	}
}

bool Stm32Flash::isBusy() const
{
	return m_isBusy;
}

void Stm32Flash::write()
{
#if defined(STM32L0) || defined(STM32F0)
	if (m_bytesToWrite % 4 != 0)
	{
		m_isBusy = false;
		SEMF_ERROR("write does not support less than 4 bytes");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Write_DataNot4ByteAligned)));
		return;
	}
#endif
#if defined(STM32F1) || defined(STM32F3)
	if (m_bytesToWrite % 2 != 0)
	{
		SEMF_ERROR("write does not support less than 2 bytes");
		m_isBusy = false;
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Write_DataNot2ByteAligned)));
		return;
	}
#endif

	HAL_StatusTypeDef state = HAL_StatusTypeDef::HAL_OK;
	if (m_bytesToWrite >= sizeof(uint32_t))
	{
		m_bytesToWrite -= sizeof(uint32_t);
		state = HAL_FLASH_Program_IT(FLASH_TYPEPROGRAM_WORD, m_addressWrite,
				*reinterpret_cast<uint32_t*>(m_dataWrite));
	}
#if !defined(STM32L0) && !defined(STM32F0)
	else if (m_bytesToWrite >= sizeof(uint16_t))
	{
		m_bytesToWrite -= sizeof(uint16_t);
		state = HAL_FLASH_Program_IT(FLASH_TYPEPROGRAM_HALFWORD, m_addressWrite,
				*reinterpret_cast<uint16_t*>(m_dataWrite));
	}
#endif
#if !defined(STM32L0) && !defined(STM32F0) && !defined(STM32F1) && !defined(STM32F3)
	else if (m_bytesToWrite == sizeof(uint8_t))
	{
		m_bytesToWrite--;
		state = HAL_FLASH_Program_IT(FLASH_TYPEPROGRAM_BYTE, m_addressWrite, *m_dataWrite);
	}
#endif
	if (state != HAL_StatusTypeDef::HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("write hal error");
			m_flash->error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Write_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("write hal busy");
			m_flash->error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Write_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("write hal timeout");
			m_flash->error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Write_HalTimeout)));
		}
		return;
	}
}

void Stm32Flash::isr()
{
	if (pFlash.ErrorCode)
	{
		semf::Stm32Flash::isrError();
		return;
	}

	if (m_flash->m_eraseIsRunning)
	{
		if (--m_flash->m_numOfSecotrsToErase)
			return;
		m_flash->m_eraseIsRunning = false;
		HAL_FLASH_Lock();
		m_flash->m_isBusy = false;
		m_flash->erased();
		return;
	}

	if (pFlash.ProcedureOnGoing != FLASH_PROC_NONE)
		return;

	HAL_StatusTypeDef state = HAL_StatusTypeDef::HAL_OK;
	if (m_flash->m_bytesToWrite >= sizeof(uint32_t))
	{
		m_flash->m_bytesToWrite -= sizeof(uint32_t);
		m_flash->m_dataWrite += sizeof(uint32_t);
		m_flash->m_addressWrite += sizeof(uint32_t);
		state = HAL_FLASH_Program_IT(FLASH_TYPEPROGRAM_WORD, m_flash->m_addressWrite,
				*reinterpret_cast<uint32_t*>(m_flash->m_dataWrite));
	}
#if !defined(STM32L0) && !defined(STM32F0)
	else if (m_flash->m_bytesToWrite >= sizeof(uint16_t))
	{
		m_flash->m_bytesToWrite -= sizeof(uint16_t);
		m_flash->m_dataWrite += sizeof(uint16_t);
		m_flash->m_addressWrite += sizeof(uint16_t);
		state = HAL_FLASH_Program_IT(FLASH_TYPEPROGRAM_HALFWORD, m_flash->m_addressWrite,
				*reinterpret_cast<uint16_t*>(m_flash->m_dataWrite));
	}
#endif
#if !defined(STM32L0) && !defined(STM32F0) && !defined(STM32F1) && !defined(STM32F3)
	else if (m_flash->m_bytesToWrite == sizeof(uint8_t))
	{
		m_flash->m_bytesToWrite--;
		m_flash->m_dataWrite++;
		m_flash->m_addressWrite++;
		state = HAL_FLASH_Program_IT(FLASH_TYPEPROGRAM_BYTE, m_flash->m_addressWrite, *m_flash->m_dataWrite);
	}
#endif
	else
	{
		HAL_FLASH_Lock();
		m_flash->m_isBusy = false;

		if (state == HAL_StatusTypeDef::HAL_OK)
		{
			SEMF_SINGLETON_INFO(m_flash, "data written");
			m_flash->dataWritten();
		}
		else
		{
			if (state == HAL_ERROR)
			{
				SEMF_SINGLETON_ERROR(m_flash, "hal error");
				m_flash->error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Isr_HalError)));
			}
			else if (state == HAL_BUSY)
			{
				SEMF_SINGLETON_ERROR(m_flash, "hal busy");
				m_flash->error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Isr_HalBusy)));
			}
			else if (state == HAL_TIMEOUT)
			{
				SEMF_SINGLETON_ERROR(m_flash, "hal timeout");
				m_flash->error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Isr_HalTimeout)));
			}
			return;
		}
	}
}

void Stm32Flash::isrError()
{
	m_flash->m_eraseIsRunning = false;
	m_flash->m_isBusy = false;
	SEMF_SINGLETON_ERROR(m_flash, "isr error");
	m_flash->error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::IsrError_InterruptError)));
}

uint16_t Stm32Flash::size() const
{
	return m_size;
}

} /* namespace semf */

#endif
