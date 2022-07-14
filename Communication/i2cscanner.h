/**
 * @file i2cscanner.h
 *
 * @date 27.10.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in 
 *            the project root directory.
 */

#ifndef COMMUNICATION_I2CSCANNER_H_
#define COMMUNICATION_I2CSCANNER_H_

#include <Interfaces/i2cmasterhardware.h>

namespace semf
{
/**
 * @brief This class scans a given address range on an I2C master hardware to identify
 * available devices.
 *
 * This is done by setting an empty write command to every address and evaluating the
 * \c I2cMasterHardware \c dataWritten and \c error signal.
 *
 * \c startScan() function call will start the scan process.
 * For every available device, the \c deviceFound signal is emitted.
 * After the total address range is tested, the \c finished signal is emitted.
 *
 * @see \c I2cScannerPrinter
 */
class I2cScanner
{
public:
	/**
	 * @brief Error codes for this class. Error ID identify a unique error() / onError call (excluding transferring).
	 */
	enum class ErrorCode : uint8_t
	{
		StartScan_IsBusy = 0
	};
	/**
	 * @brief Constructor.
	 * @param hardware I2C communication hardware to scan on.
	 */
	explicit I2cScanner(I2cMasterHardware& hardware);

	/**
	 * @brief Starts scan process for the address range from \c firstAddress to \c lastAddress.
	 * For all found devices, the \c deviceFound signal including the address of the found device is emitted.
	 * After all addresses are tested, the \c finished signal is emitted.
	 * @param firstAddress First I2C address of the address range to get tested.
	 * @param lastAddress Last I2C address of the address range to get tested.
	 * @throws StartScan_IsBusy If this is allready scanning.
	 */
	void startScan(uint8_t firstAddress = 0, uint8_t lastAddress = 0x7F);

	/**
	 * @brief Signal is emitted after an device is found while scanning.
	 * The signal delivers the I2C address of the found device.
	 */
	Signal<1, uint8_t> deviceFound;

	/**
	 * @brief Signal is emitted after all addresses in the given address range
	 * by \c startScan() function are tested.
	 */
	Signal<> finished;

	/**
	 * @brief Signal is emitted if \c startScan() function is called and I2C hardware is busy.
	 */
	Signal<1, Error> error;

private:
	/**
	 * @brief Sets the I2C address to the hardware and starts an write process with empty write data.
	 * If all addresses are tested, the \c finished signal is emitted.
	 */
	void testAddress();

	/**
	 * @brief Slot for I2C hardware's \c dataWritten signal.
	 * This function emits the \c deviceFound signal and calls the \c writeAddress() function.
	 */
	void onI2cAcknowledge();

	/**
	 * @brief Slot for I2C hardware's \c error signal.
	 * This function calls the \c writeAddress() function.
	 * @param thrown A thrown error object.
	 */
	void onI2cNotAcknowledge(Error thrown);

	/**Reference to the I2C communication hardware to scan on.*/
	I2cMasterHardware& m_hardware;
	/**First address given by \c startScan() function.*/
	uint16_t m_addressBegin = 0;
	/**One address behind the last address given by \c startScan() function.*/
	uint16_t m_addressEnd = 0;
	/**Iterates through the address range.*/
	uint16_t m_addressIterator = 0;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::I2cScanner;
};

} /* namespace semf */

#endif /* COMMUNICATION_I2CSCANNER_H_ */
