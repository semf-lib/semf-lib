/**
 * @file i2cslavehardware.cpp
 * @date 20.01.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/communication/i2cslavehardware.h>
#include <semf/utils/core/debug.h>

namespace semf
{
void I2cSlaveHardware::write(const uint8_t data[], size_t dataSize)
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
	if (!isListening())
	{
		SEMF_WARNING("slave is not listening");
	}

	m_isBusy = true;
	writeHardware(data, dataSize);
}

void I2cSlaveHardware::read(uint8_t buffer[], size_t bufferSize)
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
	if (!isListening())
	{
		SEMF_WARNING("slave is not listening");
	}

	m_isBusy = true;
	readHardware(buffer, bufferSize);
}

bool I2cSlaveHardware::isBusyReading() const
{
	return m_isBusy;
}

bool I2cSlaveHardware::isBusyWriting() const
{
	return m_isBusy;
}

void I2cSlaveHardware::setFrame(CommunicationHardware::Frame frame)
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

uint8_t I2cSlaveHardware::address() const
{
	return m_address;
}

void I2cSlaveHardware::setAddress(uint8_t address)
{
	SEMF_INFO("set address to %d", address);
	if (m_isBusy)
	{
		SEMF_ERROR("is busy");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetAddress_IsBusy)));
		return;
	}
	m_address = address;
	setAddressHardware(address);
}

void I2cSlaveHardware::setFrequency(uint32_t hz)
{
	SEMF_INFO("set frequency to %dhz", hz);
	if (m_isBusy)
	{
		SEMF_ERROR("is busy");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetFrequency_IsBusy)));
		return;
	}
	setFrequencyHardware(hz);
}

void I2cSlaveHardware::startListening()
{
	SEMF_INFO("starts to listen");
	if (m_isBusy)
	{
		SEMF_ERROR("is busy");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StartListening_IsBusy)));
		return;
	}
	startListeningHardware();
}

void I2cSlaveHardware::stopListening()
{
	SEMF_INFO("stop listening");
	if (m_isBusy)
	{
		SEMF_ERROR("is busy");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopListening_IsBusy)));
		return;
	}
	stopListeningHardware();
}

bool I2cSlaveHardware::isListening() const
{
	return m_listening;
}

void I2cSlaveHardware::setBusy(bool isBusy)
{
	m_isBusy = isBusy;
}

CommunicationHardware::Frame I2cSlaveHardware::frame() const
{
	return m_frame;
}

void I2cSlaveHardware::setListening(bool listening)
{
	m_listening = listening;
}

void I2cSlaveHardware::onDataWritten()
{
	m_isBusy = false;
	SEMF_INFO("data written");
	dataWritten();
}

void I2cSlaveHardware::onDataAvailable()
{
	m_isBusy = false;
	SEMF_INFO("data available");
	dataAvailable();
}

void I2cSlaveHardware::onError(Error thrown)
{
	m_isBusy = false;
	SEMF_ERROR("error");
	error(thrown);
}
} /* namespace semf */
