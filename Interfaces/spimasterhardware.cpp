/**
 * @file spimasterhardware.cpp
 *
 * @date 18.07.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 * 			  For detailed information, read the license file in
 *            the project root directory.
 */
#include <Core/debug.h>
#include <Interfaces/spimasterhardware.h>
#include <cstdint>

namespace semf
{
SpiMasterHardware::SpiMasterHardware(Gpio& chipSelectPin)
	:m_chipSelectPin(&chipSelectPin)
{

}

void SpiMasterHardware::write(const uint8_t data[], size_t dataSize)
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
		SEMF_ERROR("size is 0");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Write_DataSizeIsZero)));
		return;
	}
	m_isBusy = true;

	if (m_frame == CommunicationHardware::Frame::First || m_frame == CommunicationHardware::Frame::FirstAndLast)
		 enableChipSelect();

	writeHardware(data, dataSize);
}

void SpiMasterHardware::read(uint8_t buffer[], size_t bufferSize)
{
	SEMF_INFO("buffer %p, bufferSize is %u", buffer, bufferSize);
	if (m_isBusy)
	{
		SEMF_ERROR("is busy");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Read_IsBusy)));
		return;
	}
	if (buffer == nullptr && bufferSize != 0)
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

	if (m_frame == CommunicationHardware::Frame::First || m_frame == CommunicationHardware::Frame::FirstAndLast)
		 enableChipSelect();

	readHardware(buffer, bufferSize);
}

bool SpiMasterHardware::isBusyReading() const
{
	return m_isBusy;
}

bool SpiMasterHardware::isBusyWriting() const
{
	return m_isBusy;
}

void SpiMasterHardware::setFrame(CommunicationHardware::Frame frame)
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

void SpiMasterHardware::setFormat(uint8_t bits, SpiMasterHardware::TransmissionMode transmission, SpiMasterHardware::WireMode wire)
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

void SpiMasterHardware::stopWrite()
{
	if (m_wire == WireMode::ReadOnlyWires)
	{
		SEMF_ERROR("is configured as read only");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopWrite_IsReadOnly)));
		return;
	}
	if (!isBusyWriting())
	{
		SEMF_WARNING("hardware is not writing");
		return;
	}

	stopWriteHardware();
}

void SpiMasterHardware::stopRead()
{
	if (m_wire == WireMode::WriteOnlyWires)
	{
		SEMF_ERROR("is configured as write only");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopRead_IsWriteOnly)));
		return;
	}
	if (!isBusyReading())
	{
		SEMF_WARNING("hardware is not writing");
		return;
	}

	stopReadHardware();
}

void SpiMasterHardware::writeRead(const uint8_t writeData[], uint8_t readBuffer[], size_t size)
{
	SEMF_INFO("write data %p, read data %p, size is %u", writeData, readBuffer, size);
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

	if (m_frame == CommunicationHardware::Frame::First || m_frame == CommunicationHardware::Frame::FirstAndLast)
		 enableChipSelect();

	writeReadHardware(writeData, readBuffer, size);
}

void SpiMasterHardware::setChipSelectPin(Gpio& chipSelectPin)
{
	SEMF_INFO("set chip select pin");
	if (m_isBusy)
	{
		SEMF_ERROR("set chip select pin, is busy");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetChipSelectPin_IsBusy)));
		return;
	}
	m_chipSelectPin = &chipSelectPin;
}

void SpiMasterHardware::enableChipSelect()
{
	if (m_chipSelectPin != nullptr)
	{
		SEMF_INFO("enable chip select pin");
		m_chipSelectPin->reset();
	}
}

void SpiMasterHardware::disableChipSelect()
{
	if (m_chipSelectPin != nullptr)
	{
		SEMF_INFO("disable chip select pin");
		m_chipSelectPin->set();
	}
}

void SpiMasterHardware::setBusy(bool isBusy)
{
	m_isBusy = isBusy;
}

CommunicationHardware::Frame SpiMasterHardware::frame() const
{
	return m_frame;
}

void SpiMasterHardware::onDataWritten()
{
	m_isBusy = false;
	SEMF_INFO("data written");
	dataWritten();
}

void SpiMasterHardware::onDataAvailable()
{
	m_isBusy = false;
	SEMF_INFO("data available");
	dataAvailable();
}

void SpiMasterHardware::onError(Error thrown)
{
	m_isBusy = false;
	SEMF_ERROR("error");
	disableChipSelect();
	error(thrown);
}

void SpiMasterHardware::setTransmissionMode(SpiMasterHardware::TransmissionMode mode)
{
	m_transmission = mode;
}

void SpiMasterHardware::setWireMode(SpiMasterHardware::WireMode mode)
{
	m_wire = mode;
}

}  /* namespace semf */
