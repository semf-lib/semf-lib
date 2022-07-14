/**
 * @file i2cscanner.cpp
 *
 * @date 27.10.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in 
 *            the project root directory.
 */

#include <Communication/i2cscanner.h>
#include <Core/debug.h>

namespace semf
{
I2cScanner::I2cScanner(I2cMasterHardware& hardware)
	:m_hardware(hardware)
{

}

void I2cScanner::startScan(uint8_t firstAddress, uint8_t lastAddress)
{
	SEMF_INFO("I2c Scanner - Start");
	if (m_hardware.isBusyWriting())
	{
		SEMF_ERROR("is busy");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::StartScan_IsBusy)));
		return;
	}

	m_addressBegin = firstAddress;
	m_addressEnd = lastAddress + 1;
	m_addressIterator = firstAddress;

	m_hardware.error.reconnect(this, &I2cScanner::onI2cNotAcknowledge);
	m_hardware.addressFound.reconnect(this, &I2cScanner::onI2cAcknowledge);

	testAddress();
}

void I2cScanner::testAddress()
{
	if (m_addressIterator >= m_addressEnd)
	{
		SEMF_INFO("I2c Scanner - Finished\n");
		finished();
	}
	else
	{
		m_hardware.checkAddress(m_addressIterator);
	}
}

void I2cScanner::onI2cAcknowledge()
{
	SEMF_INFO("I2c device found on: 0x%02x\n", m_addressIterator);
	deviceFound(m_addressIterator++);
	testAddress();
}

void I2cScanner::onI2cNotAcknowledge(Error thrown)
{
	(void)thrown;
	m_addressIterator++;
	testAddress();
}

} /* namespace semf */
