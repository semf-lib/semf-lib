/**
 * @file i2cmasterhardware.cpp
 * @date 25.02.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/communication/i2cmasterhardware.h>
#include <semf/utils/core/debug.h>

namespace semf
{
void I2cMasterHardware::write(const uint8_t data[], size_t dataSize)
{
	SEMF_INFO("data %p, dataSize is %u", data, dataSize);
	if (m_isBusy)
	{
		SEMF_ERROR("is busy");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Write_IsBusy)));
		return;
	}
	if (data == nullptr)
	{
		SEMF_ERROR("data is nullptr");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Write_DataIsNullptr)));
		return;
	}
	if (dataSize == 0)
	{
		SEMF_ERROR("dataSize is 0");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Write_DataSizeIsZero)));
		return;
	}

	m_isBusy = true;
	writeHardware(data, dataSize);
}

void I2cMasterHardware::read(uint8_t buffer[], size_t bufferSize)
{
	SEMF_INFO("buffer %p, bufferSize is %u", buffer, bufferSize);
	if (m_isBusy)
	{
		SEMF_ERROR("is busy");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Read_IsBusy)));
		return;
	}
	if (buffer == nullptr)
	{
		SEMF_ERROR("buffer is nullptr");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Read_BufferIsNullptr)));
		return;
	}
	if (bufferSize == 0)
	{
		SEMF_ERROR("bufferSize is 0");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Read_BufferSizeIsZero)));
		return;
	}

	m_isBusy = true;
	readHardware(buffer, bufferSize);
}

bool I2cMasterHardware::isBusyReading() const
{
	return m_isBusy;
}

bool I2cMasterHardware::isBusyWriting() const
{
	return m_isBusy;
}

uint8_t I2cMasterHardware::address() const
{
	return m_address;
}

void I2cMasterHardware::setAddress(uint8_t address)
{
	SEMF_ERROR("set address %u", address);
	if (m_isBusy)
	{
		SEMF_ERROR("set address, is busy");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetAddress_IsBusy)));
		return;
	}
	m_address = address;
}

void I2cMasterHardware::setFrame(CommunicationHardware::Frame frame)
{
	m_frame = frame;
	if (m_frame == CommunicationHardware::Frame::First)
	{
		SEMF_INFO("frame mode: first");
	}
	else if (m_frame == CommunicationHardware::Frame::Next)
	{
		SEMF_INFO("frame mode: next");
	}
	else if (m_frame == CommunicationHardware::Frame::Last)
	{
		SEMF_INFO("frame mode: last");
	}
	else
	{
		SEMF_INFO("frame mode: first and last");
	}
}

void I2cMasterHardware::setBusy(bool isBusy)
{
	m_isBusy = isBusy;
}

CommunicationHardware::Frame I2cMasterHardware::frame() const
{
	return m_frame;
}

void I2cMasterHardware::onDataWritten()
{
	m_isBusy = false;
	SEMF_INFO("data written");
	dataWritten();
}

void I2cMasterHardware::onDataAvailable()
{
	m_isBusy = false;
	SEMF_INFO("data available");
	dataAvailable();
}

void I2cMasterHardware::onError(Error thrown)
{
	m_isBusy = false;
	SEMF_ERROR("error");
	error(thrown);
}
} /* namespace semf */
