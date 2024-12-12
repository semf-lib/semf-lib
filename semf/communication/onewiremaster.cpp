/**
 * @file onewiremaster.cpp
 * @date Jul 19, 2024
 * @author cr, vb
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/communication/onewiremaster.h>
#include <semf/utils/core/debug.h>

namespace semf
{
void OneWireMaster::init()
{
	SEMF_INFO("Not implemented.");
}

void OneWireMaster::deinit()
{
	SEMF_INFO("Not implemented.");
}

void OneWireMaster::sendResetCondition()
{
	if (isBusyReading() || isBusyWriting())
	{
		SEMF_ERROR("is busy");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Reset_IsBusy)));
		return;
	}

	setIsBusyReading(true);
	setIsBusyWriting(true);
	resetted.connect(m_onResettedSlot);
	reset();
}

void OneWireMaster::write(const uint8_t data[], size_t dataSize)
{
	SEMF_INFO("data at %p , dataSize is %u", data, dataSize);
	if (isBusyWriting())
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

	m_currentWriteData = data;
	m_currentBufferSize = dataSize;
	m_currentBit = 0;

	bitWritten.connect(m_onBitWrittenSlot);
	setIsBusyWriting(true);
	writeBit(prepareNextWriteBit());
}

void OneWireMaster::read(uint8_t buffer[], size_t bufferSize)
{
	SEMF_INFO("buffer at %p, bufferSize is %u", buffer, bufferSize);
	if (isBusyReading())
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

	m_currentReadBuffer = buffer;
	m_currentBufferSize = bufferSize;
	m_currentBit = 0;

	bitRead.connect(m_onBitReadSlot);
	setIsBusyReading(true);
	readBit();
}

void OneWireMaster::stopWrite()
{
	if (!m_isBusyWriting)
	{
		SEMF_ERROR("onewire does not write");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopWrite_IsNotWriting)));
		return;
	}

	resetWrite();
	writeStopped();
}

bool OneWireMaster::isBusyReading() const
{
	return m_isBusyReading;
}

bool OneWireMaster::isBusyWriting() const
{
	return m_isBusyWriting;
}

void OneWireMaster::setIsBusyReading(bool isBusy)
{
	m_isBusyReading = isBusy;
}

void OneWireMaster::setIsBusyWriting(bool isBusy)
{
	m_isBusyWriting = isBusy;
}

void OneWireMaster::stopRead()
{
	if (!m_isBusyReading)
	{
		SEMF_ERROR("onewire does not read");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StopRead_IsNotReading)));
		return;
	}

	resetRead();
	readStopped();
}

void OneWireMaster::onBitWritten()
{
	const size_t dataBitLength = m_currentBufferSize * kBitsInByte;

	if (m_currentBit >= dataBitLength)
	{
		resetWrite();
		dataWritten();
	}
	else
	{
		writeBit(prepareNextWriteBit());
	}
}

void OneWireMaster::onBitRead(bool bit)
{
	const size_t currentBitInByte = m_currentBit % kBitsInByte;

	if (bit)
		m_currentlyReadByte |= static_cast<uint8_t>(1 << currentBitInByte);

	if (currentBitInByte == kBitsInByte - 1)
	{
		m_currentReadBuffer[m_currentBit / kBitsInByte] = m_currentlyReadByte;
		m_currentlyReadByte = 0;
	}

	m_currentBit++;

	if (m_currentBit >= m_currentBufferSize * kBitsInByte)
	{
		resetRead();
		dataAvailable();
	}
	else
	{
		readBit();
	}
}

void OneWireMaster::onResetted(bool slavePresence)
{
	setIsBusyReading(false);
	setIsBusyWriting(false);

	SEMF_INFO("Resetted %u", slavePresence);
	presenceDetected(slavePresence);
}

bool OneWireMaster::prepareNextWriteBit()
{
	const size_t currentByte = m_currentWriteData[m_currentBit / kBitsInByte];
	const size_t currentBitInByte = m_currentBit % kBitsInByte;
	const bool nextBit = (currentByte >> currentBitInByte) & 1;

	m_currentBit++;

	return nextBit;
}

void OneWireMaster::resetWrite()
{
	bitWritten.disconnect(m_onBitWrittenSlot);
	m_currentWriteData = nullptr;
	m_currentBufferSize = 0;
	m_currentBit = 0;
	setIsBusyWriting(false);
}

void OneWireMaster::resetRead()
{
	bitRead.disconnect(m_onBitReadSlot);
	m_currentReadBuffer = nullptr;
	m_currentBufferSize = 0;
	m_currentBit = 0;
	setIsBusyReading(false);
}

}  // namespace semf
