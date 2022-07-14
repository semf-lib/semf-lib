/**
 * @file stm32i2cslave.h
 *
 * @date 20.01.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <HardwareAbstraction/Stm32/stm32i2cslave.h>
#include <Core/debug.h>

#if defined(STM32) && defined(HAL_I2C_MODULE_ENABLED)

namespace semf
{
LinkedQueue<Stm32I2cSlave> Stm32I2cSlave::m_queue;

Stm32I2cSlave::Stm32I2cSlave(I2C_HandleTypeDef& hwHandle)
: m_hwHandle{&hwHandle}
{
	m_queue.push(*this);
}

void Stm32I2cSlave::init()
{
	HAL_StatusTypeDef error = HAL_I2C_Init(m_hwHandle);
	if (error != HAL_OK)
	{
		if (error == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_HalError)));
		}
		else if (error == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_HalBusy)));
		}
		else if (error == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_HalTimeout)));
		}
		return;
	}
}

void Stm32I2cSlave::deinit()
{
	HAL_StatusTypeDef error = HAL_I2C_DeInit(m_hwHandle);
	if (error != HAL_OK)
	{
		if (error == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Deinit_HalError)));
		}
		else if (error == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Deinit_HalBusy)));
		}
		else if (error == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Deinit_HalTimeout)));
		}
		return;
	}
}

void Stm32I2cSlave::stopRead()
{
	SEMF_WARNING("this operation is not supported");
}

void Stm32I2cSlave::stopWrite()
{
	SEMF_WARNING("this operation is not supported");
}

void Stm32I2cSlave::setFrequency(uint32_t hz)
{
	if (isBusyReading() || isBusyWriting())
	{
		SEMF_ERROR("is busy");
		onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetFrequency_IsBusy)));
		return;
	}
	setFrequencyHardware(hz);
}

void Stm32I2cSlave::systemIsrRead(I2C_HandleTypeDef& hwHandle)
{
	for (auto& i2c : m_queue)
	{
		if (i2c.m_hwHandle == &hwHandle)
		{
			i2c.onDataAvailable();
		}
	}
}

void Stm32I2cSlave::systemIsrWritten(I2C_HandleTypeDef& hwHandle)
{
	for (auto& i2c : m_queue)
	{
		if (i2c.m_hwHandle == &hwHandle)
		{
			i2c.onDataWritten();
		}
	}
}

void Stm32I2cSlave::systemIsrListen(I2C_HandleTypeDef& hwHandle)
{
	for (auto& i2c : m_queue)
	{
		if (i2c.m_hwHandle == &hwHandle)
		{
			HAL_StatusTypeDef error = HAL_I2C_EnableListen_IT(i2c.m_hwHandle);
			if (error != HAL_OK)
			{
				if (error == HAL_ERROR)
				{
					SEMF_SINGLETON_ERROR(&i2c, "hal error");
					i2c.onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SystemIsrListen_EnableListenHalError)));
				}
				else if (error == HAL_BUSY)
				{
					SEMF_SINGLETON_ERROR(&i2c, "hal busy");
					i2c.onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SystemIsrListen_EnableListenHalBusy)));
				}
				else if (error == HAL_TIMEOUT)
				{
					SEMF_SINGLETON_ERROR(&i2c, "hal timeout");
					i2c.onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SystemIsrListen_EnableListenHalTimeout)));
				}
				return;
			}
		}
	}
}

void Stm32I2cSlave::systemIsrError(I2C_HandleTypeDef& hwHandle)
{
	for (auto& i2c : m_queue)
	{
		if (i2c.m_hwHandle == &hwHandle)
			i2c.onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SystemIsrError_HalError)));
	}
}

void Stm32I2cSlave::systemIsrAddressing(I2C_HandleTypeDef& hwHandle, uint8_t direction, uint16_t addrMatchCode)
{
	for (auto& i2c : m_queue)
	{
		if (i2c.m_hwHandle == &hwHandle)
		{
			if (direction == I2C_DIRECTION_TRANSMIT)
				i2c.readExpected();
			else if (direction == I2C_DIRECTION_RECEIVE)
				i2c.writeExpected();
		}
	}
}

void Stm32I2cSlave::readHardware(uint8_t buffer[], size_t bufferSize)
{
	SEMF_INFO("data %p, size %u", buffer, bufferSize);
	__HAL_UNLOCK(m_hwHandle);
	HAL_StatusTypeDef error = HAL_I2C_Slave_Seq_Receive_IT(m_hwHandle, buffer, bufferSize, hardwareFrame(frame()));
	if (error != HAL_OK)
	{
		if (error == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReadHardware_HalError)));
		}
		else if (error == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReadHardware_HalBusy)));
		}
		else if (error == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReadHardware_HalTimeout)));
		}
		return;
	}
}

void Stm32I2cSlave::writeHardware(const uint8_t data[], size_t dataSize)
{
	SEMF_INFO("data %p, size %u", data, dataSize);
	__HAL_UNLOCK(m_hwHandle);
	HAL_StatusTypeDef error = HAL_I2C_Slave_Seq_Transmit_IT(m_hwHandle, const_cast<uint8_t*>(data), dataSize, hardwareFrame(frame()));
	if (error != HAL_OK)
	{
		if (error == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteHardware_HalError)));
		}
		else if (error == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteHardware_HalBusy)));
		}
		else if (error == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteHardware_HalTimeout)));
		}
		return;
	}
}

void Stm32I2cSlave::startListeningHardware()
{
	HAL_StatusTypeDef error = HAL_I2C_EnableListen_IT(m_hwHandle);
	if (error != HAL_OK)
	{
		if (error == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StartListeningHardware_HalError)));
		}
		else if (error == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StartListeningHardware_HalBusy)));
		}
		else if (error == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StartListeningHardware_HalTimeout)));
		}
		return;
	}
	setListening(true);
}

void Stm32I2cSlave::stopListeningHardware()
{
	HAL_StatusTypeDef error = HAL_I2C_DisableListen_IT(m_hwHandle);
	if (error != HAL_OK)
	{
		if (error == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopListeningHardware_HalError)));
		}
		else if (error == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopListeningHardware_HalError)));
		}
		else if (error == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopListeningHardware_HalError)));
		}
		return;
	}
	setListening(false);
}

void Stm32I2cSlave::setAddressHardware(uint8_t address)
{
	deinit();
	m_hwHandle->Init.OwnAddress1 = address << 1;
	init();
}

uint32_t Stm32I2cSlave::hardwareFrame(CommunicationHardware::Frame frame) const
{
	if (frame == CommunicationHardware::Frame::First)
	{
		return I2C_FIRST_FRAME;
	}
	else if (frame == CommunicationHardware::Frame::FirstAndLast)
	{
		return I2C_FIRST_AND_LAST_FRAME;
	}
	else if (frame == CommunicationHardware::Frame::Next)
	{
		return I2C_NEXT_FRAME;
	}
	else  // CommunicationHardware::Frame::LAST
	{
		return I2C_LAST_FRAME;
	}
}

I2C_HandleTypeDef* Stm32I2cSlave::hardwareHandle()
{
	return m_hwHandle;
}

}  // namespace semf

#endif
