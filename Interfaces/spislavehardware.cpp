/**
 * @file spislavehardware.cpp
 *
 * @date 26.01.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in 
 *            the project root directory.
 */

#include <Interfaces/spislavehardware.h>
#include <Core/debug.h>

namespace semf
{
void SpiSlaveHardware::write(const uint8_t data[], size_t dataSize)
{
	SEMF_INFO("data at %p , dataSize is %u", data, dataSize);
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

void SpiSlaveHardware::read(uint8_t buffer[], size_t bufferSize)
{
	SEMF_INFO("buffer at %p, bufferSize is %u", buffer, bufferSize);
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

bool SpiSlaveHardware::isBusyReading() const
{
	return m_isBusy;
}

bool SpiSlaveHardware::isBusyWriting() const
{
	return m_isBusy;
}

void SpiSlaveHardware::stopRead()
{
	if (!isBusyReading())
	{
		SEMF_ERROR("hardware does not read");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopRead_IsNotReading)));
		return;
	}
	stopReadHardware();
}

void SpiSlaveHardware::stopWrite()
{
	if (!isBusyWriting())
	{
		SEMF_ERROR("hardware does not read");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopWrite_IsNotWriting)));
		return;
	}
	stopWriteHardware();
}

void SpiSlaveHardware::setFrame(SpiSlaveHardware::Frame frame)
{
	(void)frame;
	SEMF_WARNING("not implemented");
}

void SpiSlaveHardware::writeRead(const uint8_t writeData[], uint8_t readBuffer[], size_t size)
{
	SEMF_INFO("writeData at %p, readBuffer at %p, size is %u", writeData, readBuffer, size);
	if (m_isBusy)
	{
		SEMF_ERROR("is busy");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteRead_IsBusy)));
		return;
	}
	if (writeData == nullptr)
	{
		SEMF_ERROR("write data is nullptr");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteRead_WriteDataIsNullptr)));
		return;
	}
	if (readBuffer == nullptr)
	{
		SEMF_ERROR("read buffer is nullptr");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteRead_ReadBufferIsNullptr)));
		return;
	}
	if (size == 0)
	{
		SEMF_ERROR("size is 0");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::WriteRead_SizeIsZero)));
		return;
	}

	m_isBusy = true;
	writeReadHardware(writeData, readBuffer, size);
}

void SpiSlaveHardware::setFormat(uint8_t bits, TransmissionMode transmission, WireMode wire)
{
	if (m_isBusy)
	{
		SEMF_ERROR("is busy");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetFormat_IsBusy)));
		return;
	}

	m_transmission = transmission;
	m_wire = wire;

	deinit();
	setFormatHardware(bits, transmission, wire);
	init();
}

void SpiSlaveHardware::onError(Error thrown)
{
	SEMF_ERROR("error");
	m_isBusy = false;
	error(thrown);
}

void SpiSlaveHardware::onDataAvailable()
{
	SEMF_INFO("data available");
	m_isBusy = false;
	dataAvailable();
}

void SpiSlaveHardware::onDataWritten()
{
	SEMF_INFO("data written");
	m_isBusy = false;
	dataWritten();
}

void SpiSlaveHardware::setBusy(bool busy)
{
	m_isBusy = busy;
}

void SpiSlaveHardware::setWireMode(SpiSlaveHardware::WireMode mode)
{
	m_wire = mode;
}

void SpiSlaveHardware::setTransmissionMode(SpiSlaveHardware::TransmissionMode mode)
{
	m_transmission = mode;
}
}  // namespace semf
