/**
 * @file softi2cmaster.cpp
 * @date 20.08.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/communication/softi2cmaster.h>
#include <semf/utils/core/debug.h>

namespace semf
{
SoftI2cMaster::SoftI2cMaster(Gpio& scl, Gpio& sda, app::Timer& timer)
: m_scl(scl),
  m_sda(sda),
  m_timer(timer)
{
}

void SoftI2cMaster::init()
{
	SEMF_INFO("start init");
	m_timer.timeout.connect(m_timemoutSlot);
	m_scl.set();
	m_sda.set();
	m_scl.setDirection(Gpio::Direction::OutputOpendrain);
	m_sda.setDirection(Gpio::Direction::OutputOpendrain);
	m_scl.set();
	m_sda.set();
	m_bitIndex = 7;
	m_lastFrame = CommunicationHardware::Frame::FirstAndLast;
	m_lastOperationWasWrite = false;
	m_acknowledgeError = false;
	m_data = nullptr;
	m_dataSize = 0;
	m_dataIndex = 0;
}

void SoftI2cMaster::deinit()
{
	SEMF_INFO("start deinit");
	m_timer.timeout.clear();
	m_scl.setDirection(Gpio::Direction::Input);
	m_sda.setDirection(Gpio::Direction::Input);
}

void SoftI2cMaster::stopWrite()
{
	SEMF_INFO("stop write");
	deinit();
	init();
}

void SoftI2cMaster::stopRead()
{
	SEMF_INFO("stop read");
	deinit();
	init();
}

void SoftI2cMaster::writeHardware(const uint8_t data[], size_t size)
{
	SEMF_INFO("write data %p with size %u", data, size);
	m_data = const_cast<uint8_t*>(data);
	m_dataSize = size;
	m_dataIndex = 0;
	m_bitIndex = 7;
	m_acknowledgeError = false;

	if (frame() == CommunicationHardware::Frame::First || frame() == CommunicationHardware::Frame::FirstAndLast)
	{
		m_activeByte = address() << 1;
		m_writingAddress = true;
		startConditionResetSda();
	}
	else
	{
		writeByte();
	}
	m_lastFrame = frame();
	m_lastOperationWasWrite = true;
}

void SoftI2cMaster::readHardware(uint8_t buffer[], size_t bufferSize)
{
	SEMF_INFO("read data %p with size %u", buffer, bufferSize);
	m_data = buffer;
	m_dataSize = bufferSize;
	m_dataIndex = 0;
	m_bitIndex = 7;
	m_acknowledgeError = false;

	if (frame() == CommunicationHardware::Frame::First || frame() == CommunicationHardware::Frame::FirstAndLast)
	{
		m_activeByte = static_cast<uint8_t>((address() << 1) | 0x01);
		m_writingAddress = true;
		startConditionResetSda();
	}
	else if (m_lastOperationWasWrite && (m_lastFrame == CommunicationHardware::Frame::First || m_lastFrame == CommunicationHardware::Frame::Next))
	{
		m_activeByte = static_cast<uint8_t>((address() << 1) | 0x01);
		m_writingAddress = true;
		resartConditionSetSdaResetScl();
	}
	else
	{
		readByte();
	}
	m_lastFrame = frame();
	m_lastOperationWasWrite = false;
}

void SoftI2cMaster::resartConditionSetSdaResetScl()
{
	SEMF_INFO("do restart condition (followed by start condition)");
	m_sda.set();
	m_scl.reset();
	m_timemoutSlot.setFunction(SEMF_SLOT_FUNC(resartConditionSetScl));
}

void SoftI2cMaster::resartConditionSetScl()
{
	m_scl.set();
	m_timemoutSlot.setFunction(SEMF_SLOT_FUNC(startConditionResetSda));
}

void SoftI2cMaster::startConditionResetSda()
{
	SEMF_INFO("do start condition");
	m_sda.reset();
	m_timemoutSlot.setFunction(SEMF_SLOT_FUNC(startConditionResetScl));
}

void SoftI2cMaster::startConditionResetScl()
{
	m_scl.reset();
	writeByte();
}

void SoftI2cMaster::writeByte()
{
	if (m_writingAddress)
	{
		SEMF_INFO("start write address: %u", m_activeByte);
	}
	else
	{
		m_activeByte = m_data[m_dataIndex];
		SEMF_INFO("start write byte: %u", m_activeByte);
	}
	writeByteSetSdaDataBit();
}

void SoftI2cMaster::writeByteSetSdaDataBit()
{
	// Set or reset sda
	if (m_activeByte & (1 << m_bitIndex))
		m_sda.set();
	else
		m_sda.reset();
	m_timemoutSlot.setFunction(SEMF_SLOT_FUNC(writeByteSetScl));
}

void SoftI2cMaster::writeByteSetScl()
{
	m_scl.set();
	m_timemoutSlot.setFunction(SEMF_SLOT_FUNC(writeByteResetScl));
}

void SoftI2cMaster::writeByteResetScl()
{
	m_scl.reset();
	m_timer.timeout.clear();
	// Byte not finished -> next bit and goto onWriteByteSetSclLow
	if (m_bitIndex > 0)
	{
		m_bitIndex--;
		writeByteSetSdaDataBit();
	}
	// Byte finished -> goto checkAcknolage
	else
	{
		m_bitIndex = 7;
		checkAcknowledgeSetSdaInput();
	}
}

void SoftI2cMaster::readByte()
{
	SEMF_INFO("start read byte");
	m_sda.set();
	m_sda.setDirection(Gpio::Direction::Input);
	m_activeByte = 0x00;
	readByteReadSdaDataBit();
}

void SoftI2cMaster::readByteReadSdaDataBit()
{
	// Read bit status
	if (m_sda.state())
	{
		m_activeByte = static_cast<uint8_t>(m_activeByte | (1 << m_bitIndex));
	}
	m_timemoutSlot.setFunction(SEMF_SLOT_FUNC(readByteSetScl));
}

void SoftI2cMaster::readByteSetScl()
{
	m_scl.set();
	m_timemoutSlot.setFunction(SEMF_SLOT_FUNC(readByteResetScl));
}

void SoftI2cMaster::readByteResetScl()
{
	m_timer.timeout.clear();
	m_scl.reset();
	if (m_bitIndex > 0)
	{
		m_bitIndex--;
		// Read bit status
		if (m_sda.state())
			m_activeByte = static_cast<uint8_t>(m_activeByte | (1 << m_bitIndex));
		readByteReadSdaDataBit();
	}
	// Byte finished -> goto checkAcknolage
	else
	{
		m_bitIndex = 7;
		setAcknowledgeSetSdaOutput();
	}
}

void SoftI2cMaster::checkAcknowledgeSetSdaInput()
{
	m_sda.set();
	m_sda.setDirection(Gpio::Direction::Input);
	m_timemoutSlot.setFunction(SEMF_SLOT_FUNC(checkAcknowledgeSetScl));
}

void SoftI2cMaster::checkAcknowledgeSetScl()
{
	m_scl.set();
	m_acknowledgeBit = m_sda.state();
	m_timemoutSlot.setFunction(SEMF_SLOT_FUNC(checkAcknowledgeResetScl));
}

void SoftI2cMaster::checkAcknowledgeResetScl()
{
	m_scl.reset();
	m_timer.timeout.clear();
	m_sda.setDirection(Gpio::Direction::OutputOpendrain);
	finishAcknowledge();
}

void SoftI2cMaster::setAcknowledgeSetSdaOutput()
{
	m_sda.setDirection(Gpio::Direction::OutputOpendrain);
	if (m_dataIndex + 1 < m_dataSize)
		m_sda.reset();
	m_timemoutSlot.setFunction(SEMF_SLOT_FUNC(setAcknowledgeSetScl));
}

void SoftI2cMaster::setAcknowledgeSetScl()
{
	m_scl.set();
	m_timemoutSlot.setFunction(SEMF_SLOT_FUNC(setAcknowledgeResetScl));
}

void SoftI2cMaster::setAcknowledgeResetScl()
{
	m_timer.timeout.clear();
	m_scl.reset();
	finishAcknowledge();
}

void SoftI2cMaster::finishAcknowledge()
{
	if (m_writingAddress)
	{
		m_writingAddress = false;
	}
	else
	{
		if (isBusyReading())
			m_data[m_dataIndex] = m_activeByte;
		m_dataIndex++;
	}

	if (isBusyReading())
		finishAcknowledgeReadOperation();
	else
		finishAcknowledgeWriteOperation();
}

void SoftI2cMaster::finishAcknowledgeWriteOperation()
{
	// Acknowledge error -> Do stop condition and finish with error
	if (m_acknowledgeBit)
	{
		m_acknowledgeError = true;
		SEMF_INFO("nack");
		stopConditionResetSda();
		return;
	}

	// Finished
	if (m_dataIndex == m_dataSize)
	{
		if (frame() == CommunicationHardware::Frame::Last || frame() == CommunicationHardware::Frame::FirstAndLast)
		{
			stopConditionResetSda();
		}
		else
		{
			SEMF_INFO("all bytes written");
			onDataWritten();
		}
	}
	// Write next byte
	else
	{
		writeByte();
	}
}

void SoftI2cMaster::finishAcknowledgeReadOperation()
{
	// Finished
	if (m_dataIndex == m_dataSize)
	{
		m_sda.setDirection(Gpio::Direction::OutputPushpull);
		m_sda.set();
		if (frame() == CommunicationHardware::Frame::Last || frame() == CommunicationHardware::Frame::FirstAndLast)
		{
			stopConditionResetSda();
		}
		else
		{
			SEMF_INFO("all bytes read");
			onDataAvailable();
		}
	}
	// Read next byte
	else
	{
		readByte();
	}
}

void SoftI2cMaster::stopConditionResetSda()
{
	SEMF_INFO("do stop condition");
	m_sda.reset();
	m_timemoutSlot.setFunction(SEMF_SLOT_FUNC(stopConditionSetScl));
}

void SoftI2cMaster::stopConditionSetScl()
{
	m_scl.reset();
	m_scl.set();
	m_timemoutSlot.setFunction(SEMF_SLOT_FUNC(stopConditionSetSda));
}

void SoftI2cMaster::stopConditionSetSda()
{
	m_timer.timeout.clear();
	m_sda.set();
	// Read operation
	if (isBusyReading() && !m_acknowledgeError)
	{
		SEMF_INFO("all data read");
		onDataAvailable();
	}
	// Write operation
	else if (isBusyWriting() && !m_acknowledgeError)
	{
		SEMF_INFO("all data written");
		onDataWritten();
	}
	// Not allowed or acknowledge error
	else
	{
		SEMF_INFO("nack error");
		onError(Error(kSemfClassId, static_cast<uint32_t>(ErrorCode::StopConditionSetSda_NackError)));
	}
}

void SoftI2cMaster::setFrequency(uint32_t hz)
{
	(void)hz;
	SEMF_INFO("not implemented");
}
} /* namespace semf */
