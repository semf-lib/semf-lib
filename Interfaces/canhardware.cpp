/**
 * @file canhardware.cpp
 *
 * @date 24.10.2018
 * @author aa
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved For detailed information, read the license file in the project root directory.
 */

#include <Core/debug.h>
#include <Interfaces/canhardware.h>

namespace semf
{
void CanHardware::write(const uint8_t data[], size_t dataSize)
{
	SEMF_INFO("data %p, size is %u", data, dataSize);
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
		SEMF_ERROR("size is 0");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Write_DataSizeIsZero)));
		return;
	}

	m_isBusyWriting = true;
	writeHardware(data, dataSize);
}

void CanHardware::read(uint8_t buffer[], size_t bufferSize)
{
	if (buffer == nullptr)
	{
		SEMF_ERROR("data is nullptr");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Read_DataIsNullptr)));
		return;
	}
	if (bufferSize == 0)
	{
		SEMF_ERROR("size is 0");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Read_DataSizeIsZero)));
		return;
	}
	setReadBuffer(buffer, bufferSize);
}

bool CanHardware::isBusyReading() const
{
	return false;
}

bool CanHardware::isBusyWriting() const
{
	return m_isBusyWriting;
}

void CanHardware::request()
{
	SEMF_INFO("request");
	if (m_isBusyWriting)
	{
		SEMF_ERROR("is busy");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Request_IsBusy)));
		return;
	}
	m_isBusyWriting = true;
	requestHardware();
}

void CanHardware::setBusyWriting(bool isBusy)
{
	m_isBusyWriting = isBusy;
}

void CanHardware::onDataWritten()
{
	m_isBusyWriting = false;
	SEMF_INFO("data written");
	dataWritten();
}

void CanHardware::onDataAvailable()
{
	SEMF_INFO("data available");
	dataAvailable();
}

void CanHardware::onError(Error thrown)
{
	m_isBusyWriting = false;
	SEMF_ERROR("error");
	error(thrown);
}

void CanHardware::onDataRequested()
{
	dataRequested();
}

}  // namespace semf
