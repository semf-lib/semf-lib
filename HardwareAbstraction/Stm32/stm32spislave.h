/**
 * @file stm32spislave.h
 *
 * @date 27.01.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in 
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32_STM32SPISLAVE_H_
#define HARDWAREABSTRACTION_STM32_STM32SPISLAVE_H_

#include <Interfaces/spislavehardware.h>
#include <HardwareAbstraction/Stm32/stm32.h>
#include <Core/Queues/linkedqueue.h>

#if defined(STM32) && defined(HAL_SPI_MODULE_ENABLED)
namespace semf
{
/**
 * @brief Class for using the SPI peripheral in slave mode on a stm32.
 */
class Stm32SpiSlave : public SpiSlaveHardware, public LinkedQueue<Stm32SpiSlave>::Node
{
public:
	/**
	 * @brief Error codes for this class. Error ID identify a unique error() / onError call (excluding transferring).
	 */
	enum class ErrorCode : uint8_t
	{
		Init_HalError = 0,
		Init_HalBusy,
		Init_HalTimeout,
		Deinit_HalError,
		Deinit_HalBusy,
		Deinit_HalTimeout,
		SystemIsrError_Isr,
		SetFormatHardware_BitsNotSupported,
		StopWriteHardware_HalError,
		StopWriteHardware_HalBusy,
		StopWriteHardware_HalTimeout,
		StopReadHardware_HalError,
		StopReadHardware_HalBusy,
		StopReadHardware_HalTimeout,
		WriteHardware_HalError,
		WriteHardware_HalBusy,
		WriteHardware_HalTimeout,
		ReadHardware_HalError,
		ReadHardware_HalBusy,
		ReadHardware_HalTimeout,
		WriteReadHardware_HalError,
		WriteReadHardware_HalBusy,
		WriteReadHardware_HalTimeout
	};
	/**
	 * @brief Constructor.
	 * @param hwHandle Native hardware handle.
	 */
	explicit Stm32SpiSlave(SPI_HandleTypeDef& hwHandle);
	/**
	 * @copydoc Stm32SpiSlave::init()
	 * @throws Init_HalError If the ST-HAL returns a hal error.
	 * @throws Init_HalBusy If the ST-HAL returns a hal busy.
	 * @throws Init_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void init();
	/**
	 * @copydoc Stm32SpiSlave::deinit()
	 * @throws Deinit_HalError If the ST-HAL returns a hal error.
	 * @throws Deinit_HalBusy If the ST-HAL returns a hal busy.
	 * @throws Deinit_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void deinit();
	/**
	 * @brief Handles the global receive complete callback.
	 * @param hwHandle The handle causing the callback.
	 */
	static void systemIsrRead(SPI_HandleTypeDef& hwHandle);
	/**
	 * @brief Handles the global transmit complete callback.
	 * @param hwHandle The handle causing the callback.
	 */
	static void systemIsrWritten(SPI_HandleTypeDef& hwHandle);
	/**
	 * @brief Handles the global receive transmit complete callback.
	 * @param hwHandle The handle causing the callback.
	 */
	static void systemIsrWrittenAndRead(SPI_HandleTypeDef& hwHandle);
	/**
	 * @brief Handles the global error callback.
	 * @param hwHandle The handle causing the callback.
	 */
	/**
	 * @copydoc Stm32SpiSlave::deinit()
	 * @throws SystemIsrError_HalError If the ST-HAL returns a hal error.
	 * @throws SystemIsrError_HalBusy If the ST-HAL returns a hal busy.
	 * @throws SystemIsrError_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	static void systemIsrError(SPI_HandleTypeDef& hwHandle);
	/**
	 * @brief Handles the global abort callback.
	 * @param hwHandle The handle causing the callback.
	 */
	static void systemIsrAbort(SPI_HandleTypeDef& hwHandle);
protected:
	/**
	 * @copydoc Stm32SpiSlave::writeHardware()
	 * @throws WriteHardware_HalError If the ST-HAL returns a hal error.
	 * @throws WriteHardware_HalBusy If the ST-HAL returns a hal busy.
	 * @throws WriteHardware_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void writeHardware(const uint8_t data[], size_t dataSize);
	/**
	 * @copydoc Stm32SpiSlave::readHardware()
	 * @throws ReadHardware_HalError If the ST-HAL returns a hal error.
	 * @throws ReadHardware_HalBusy If the ST-HAL returns a hal busy.
	 * @throws ReadHardware_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void readHardware(uint8_t buffer[], size_t bufferSize);
	/**
	 * @copydoc Stm32SpiSlave::writeReadHardware()
	 * @throws WriteReadHardware_HalError If the ST-HAL returns a hal error.
	 * @throws WriteReadHardware_HalBusy If the ST-HAL returns a hal busy.
	 * @throws WriteReadHardware_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void writeReadHardware(const uint8_t writeData[], uint8_t readBuffer[], size_t size);
	/**
	 * @copydoc Stm32SpiSlave::stopReadHardware()
	 * @throws StopReadHardware_HalError If the ST-HAL returns a hal error.
	 * @throws StopReadHardware_HalBusy If the ST-HAL returns a hal busy.
	 * @throws StopReadHardware_HalTimeout If the ST-HAL returns a hal timeout.
 	*/
	void stopReadHardware();
	/**
	 * @copydoc Stm32SpiSlave::stopWriteHardware()
	 * @throws StopWriteHardware_HalError If the ST-HAL returns a hal error.
	 * @throws StopWriteHardware_HalBusy If the ST-HAL returns a hal busy.
	 * @throws StopWriteHardware_HalTimeout If the ST-HAL returns a hal timeout.
 	*/
	void stopWriteHardware();
	/**
	 * @brief Emits the signal \c dataAvailable and resets the busy flag.
	 */
	void onReadWritten();
	/**
	 * @brief Handles the read written ISR.
	 */
	void onReadWrittenHardware();
	/**
	 * @brief Handles the abort ISR.
	 */
	void onAbortedHardware();
	/**
	 * @copydoc SpiSlaveHardware::setFormatHardware()
	 */
	void setFormatHardware(uint8_t bits, TransmissionMode transmission, WireMode wire);
private:
	/**Native hardware handle.*/
	SPI_HandleTypeDef* m_hwHandle;
	/**Queue conating all spi slaves.*/
	static LinkedQueue<Stm32SpiSlave> m_queue;
	/**Signal for handeling the abort callback*/
	Signal<> aborted;
	/**Current write data.*/
	uint8_t* m_writeData;
	/**Size of current write data.*/
	size_t m_writeDataSize;
	/**buffer for reading data*/
	uint8_t* m_readBuffer;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32SpiSlave;
};
}  // namespace semf
#endif
#endif /* HARDWAREABSTRACTION_STM32_STM32SPISLAVE_H_ */
