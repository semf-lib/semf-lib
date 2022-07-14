/**
 * @file uarthardware.cpp
 *
 * @date 25.02.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */
#include <Core/debug.h>
#include <Interfaces/uarthardware.h>
#include <cstdint>

namespace semf
{
void UartHardware::write(const uint8_t data[], size_t dataSize)
{
	SEMF_INFO("data %p, dataSize is %u", data, dataSize);
	if (m_isBusyWriting)
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

	m_isBusyWriting = true;
	writeHardware(data, dataSize);
}

void UartHardware::read(uint8_t buffer[], size_t bufferSize)
{
	SEMF_INFO("buffer %p, bufferSize is %u", buffer, bufferSize);
	if (m_isBusyReading)
	{
		SEMF_ERROR("is busy");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Read_IsBusy)));
		return;
	}
	if (buffer == nullptr)
	{
		SEMF_ERROR("data is nullptr");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Read_BufferIsNullptr)));
		return;
	}
	if (bufferSize == 0)
	{
		SEMF_ERROR("dataSize is 0");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Read_BufferSizeIsZero)));
		return;
	}

	m_isBusyReading = true;
	readHardware(buffer, bufferSize);
}

bool UartHardware::isBusyReading() const
{
	return m_isBusyReading;
}

bool UartHardware::isBusyWriting() const
{
	return m_isBusyWriting;
}

void UartHardware::setBusyReading(bool isBusy)
{
	m_isBusyReading = isBusy;
}

void UartHardware::setBusyWriting(bool isBusy)
{
	m_isBusyWriting = isBusy;
}

void UartHardware::onDataWritten()
{
	m_isBusyWriting = false;
	SEMF_INFO("data written");
	dataWritten();
}

void UartHardware::onDataAvailable()
{
	m_isBusyReading = false;
	SEMF_INFO("data available");
	dataAvailable();
}

void UartHardware::onError(Error thrown)
{
	m_isBusyReading = false;
	m_isBusyWriting = false;
	SEMF_ERROR("error");
	error(thrown);
}

} /* namespace semf */
