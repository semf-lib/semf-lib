/**
 * @file stm32i2cmaster.cpp
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <HardwareAbstraction/Stm32/stm32i2cmaster.h>
#include <Core/debug.h>

#if defined(STM32) && defined(HAL_I2C_MODULE_ENABLED)

namespace semf
{
LinkedQueue<Stm32I2cMaster> Stm32I2cMaster::m_queue;
Stm32I2cMaster::Stm32I2cMaster(I2C_HandleTypeDef& hwHandle)
	:m_hwHandle(&hwHandle)
{
	queue()->push(*this);
}

void Stm32I2cMaster::init()
{
	SEMF_INFO("init");
	HAL_StatusTypeDef state = HAL_I2C_Init(m_hwHandle);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_HalTimeout)));
		}
		return;
	}
}

void Stm32I2cMaster::deinit()
{
	SEMF_INFO("deinit");
	HAL_StatusTypeDef state = HAL_I2C_DeInit(m_hwHandle);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Deinit_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Deinit_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Deinit_HalTimeout)));
		}
		return;
	}
}

void Stm32I2cMaster::stopWrite()
{
	if (!isBusyWriting())
	{
		SEMF_WARNING("device was not writing");
		return;
	}
	canceled.reconnect(&writeStopped, &Signal<>::emitSignal);
	HAL_StatusTypeDef state = HAL_I2C_Master_Abort_IT(m_hwHandle, address());
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Deinit_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Deinit_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Deinit_HalTimeout)));
		}
		return;
	}
}

void Stm32I2cMaster::stopRead()
{
	if (!isBusyReading())
	{
		SEMF_WARNING("device was not reading");
		return;
	}
	canceled.reconnect(&readStopped, &Signal<>::emitSignal);
	HAL_StatusTypeDef state = HAL_I2C_Master_Abort_IT(m_hwHandle, address());
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopRead_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopRead_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopRead_HalTimeout)));
		}
		return;
	}
}

void Stm32I2cMaster::setFrequency(uint32_t hz)
{
	SEMF_INFO("set frequency to %d hz", hz);
	if (isBusyReading() || isBusyWriting())
	{
		SEMF_ERROR("is busy");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetFrequency_IsBusy)));
		return;
	}
	deinit();
	setFrequencyHardware(hz);
	init();
}

void Stm32I2cMaster::checkAddress(uint8_t address)
{
	setFrame(Frame::FirstAndLast);
	setAddress(address);

	dataAvailable.reconnect(&addressFound, &semf::Signal<>::emitSignal);

	readHardware(nullptr, 0);
}

LinkedQueue<Stm32I2cMaster>* Stm32I2cMaster::queue()
{
	static LinkedQueue<Stm32I2cMaster> queue;
	return &queue;
}

void Stm32I2cMaster::systemIsrRead(I2C_HandleTypeDef& i2c)
{
	for(Stm32I2cMaster& i : *(queue()))
		i.isrRead(i2c);
}

void Stm32I2cMaster::systemIsrWritten(I2C_HandleTypeDef& i2c)
{
	for(Stm32I2cMaster& i : *(queue()))
		i.isrWritten(i2c);
}

void Stm32I2cMaster::systemIsrError(I2C_HandleTypeDef& i2c)
{
	for(Stm32I2cMaster& i : *(queue()))
		i.isrError(i2c);
}

void Stm32I2cMaster::systemIsrAbort(I2C_HandleTypeDef& i2c)
{
	for(Stm32I2cMaster& i : *(queue()))
		i.isrAbort(i2c);
}

void Stm32I2cMaster::isrRead(I2C_HandleTypeDef& i2c)
{
	if (&i2c != m_hwHandle)
		return;

	SEMF_INFO("data available");
	onDataAvailable();
}

void Stm32I2cMaster::isrWritten(I2C_HandleTypeDef& i2c)
{
	if (&i2c != m_hwHandle)
		return;

	SEMF_INFO("data written");
	onDataWritten();
}

void Stm32I2cMaster::isrError(I2C_HandleTypeDef& i2c)
{
	if (&i2c != m_hwHandle)
		return;

	SEMF_INFO("error");
	onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::IsrError_HalError)));
}

void Stm32I2cMaster::isrAbort(I2C_HandleTypeDef& i2c)
{
	if (&i2c != m_hwHandle)
		return;

	SEMF_INFO("abort");
	setBusy(false);
	canceled();
}

I2C_HandleTypeDef* Stm32I2cMaster::hardwareHandle()
{
	return m_hwHandle;
}

uint32_t Stm32I2cMaster::hardwareFrame(CommunicationHardware::Frame frame) const
{
	uint32_t mode;
	switch(frame)
	{
	case CommunicationHardware::Frame::First:
		SEMF_INFO("frame mode: first");
		mode = I2C_FIRST_FRAME;
		break;
	case CommunicationHardware::Frame::Next:
		SEMF_INFO("frame mode: next");
		mode = I2C_NEXT_FRAME;
		break;
	case CommunicationHardware::Frame::Last:
		SEMF_INFO("frame mode: last");
		mode = I2C_LAST_FRAME;
		break;
	case CommunicationHardware::Frame::FirstAndLast:
	default:
		SEMF_INFO("frame mode: first and last");
		mode = I2C_FIRST_AND_LAST_FRAME;
		break;
	}
	return mode;
}

void Stm32I2cMaster::readHardware(uint8_t buffer[], size_t bufferSize)
{
	SEMF_INFO("data %p, size %p", buffer, bufferSize);
	__HAL_UNLOCK(m_hwHandle);
	HAL_StatusTypeDef state;
	if (frame() == Frame::FirstAndLast)
	{
		state = HAL_I2C_Master_Receive_IT(m_hwHandle, static_cast<uint16_t>(address() << 1), buffer, bufferSize);
	}
	else
	{
		state = HAL_I2C_Master_Seq_Receive_IT(m_hwHandle,
				static_cast<uint16_t>(address() << 1), buffer, bufferSize, hardwareFrame(frame()));
	}
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReadHardware_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReadHardware_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::ReadHardware_HalTimeout)));
		}
		return;
	}
}

void Stm32I2cMaster::writeHardware(const uint8_t data[], size_t dataSize)
{
	SEMF_INFO("data %p, size %u", data, dataSize);
	__HAL_UNLOCK(m_hwHandle);
	HAL_StatusTypeDef state;
	if (frame() == Frame::FirstAndLast)
	{
		state = HAL_I2C_Master_Transmit_IT(m_hwHandle, static_cast<uint16_t>(address() << 1), const_cast<uint8_t*>(data), dataSize);
	}
	else
	{
		state = HAL_I2C_Master_Seq_Transmit_IT(m_hwHandle,
				static_cast<uint16_t>(address() << 1), const_cast<uint8_t*>(data), dataSize, hardwareFrame(frame()));
	}

	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteHardware_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteHardware_HalError)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteHardware_HalError)));
		}
		return;
	}
}
} /* namespace semf */

#endif
