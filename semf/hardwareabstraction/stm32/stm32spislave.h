/**
 * @file stm32spislave.h
 * @date 27.01.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32_STM32SPISLAVE_H_
#define SEMF_HARDWAREABSTRACTION_STM32_STM32SPISLAVE_H_

#include <semf/communication/spislavehardware.h>
#include <semf/hardwareabstraction/stm32/stm32.h>
#include <semf/utils/core/queues/linkedqueue.h>
#include <semf/utils/core/signals/slot.h>

#if defined(STM32) && defined(HAL_SPI_MODULE_ENABLED)
namespace semf
{
/**
 * @brief \c SpiSlaveHardware implementation for STM32.
 */
class Stm32SpiSlave : public SpiSlaveHardware, public LinkedQueue<Stm32SpiSlave>::Node
{
public:
	/**Error codes for this class. Error ID identify a unique error() / onError call (excluding transferring).*/
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
	 * @param useDma If true - semf will use DMA HAL functions for read and write.
	 */
	explicit Stm32SpiSlave(SPI_HandleTypeDef& hwHandle, bool useDma = false);
	explicit Stm32SpiSlave(const Stm32SpiSlave& other) = delete;
	virtual ~Stm32SpiSlave() = default;

	/**
	 * @copydoc CommunicationHardware::init()
	 * @throws Init_HalError If the ST-HAL returns a hal error.
	 * @throws Init_HalBusy If the ST-HAL returns a hal busy.
	 * @throws Init_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void init() override;
	/**
	 * @copydoc CommunicationHardware::deinit()
	 * @throws Deinit_HalError If the ST-HAL returns a hal error.
	 * @throws Deinit_HalBusy If the ST-HAL returns a hal busy.
	 * @throws Deinit_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void deinit() override;
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
	static void systemIsrError(SPI_HandleTypeDef& hwHandle);
	/**
	 * @brief Handles the global abort callback.
	 * @param hwHandle The handle causing the callback.
	 */
	static void systemIsrAbort(SPI_HandleTypeDef& hwHandle);

protected:
	/**
	 * @copydoc SpiSlaveHardware::writeHardware()
	 * @throws WriteHardware_HalError If the ST-HAL returns a hal error.
	 * @throws WriteHardware_HalBusy If the ST-HAL returns a hal busy.
	 * @throws WriteHardware_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void writeHardware(const uint8_t data[], size_t dataSize) override;
	/**
	 * @copydoc SpiSlaveHardware::readHardware()
	 * @throws ReadHardware_HalError If the ST-HAL returns a hal error.
	 * @throws ReadHardware_HalBusy If the ST-HAL returns a hal busy.
	 * @throws ReadHardware_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void readHardware(uint8_t buffer[], size_t bufferSize) override;
	/**
	 * @copydoc SpiSlaveHardware::writeReadHardware()
	 * @throws WriteReadHardware_HalError If the ST-HAL returns a hal error.
	 * @throws WriteReadHardware_HalBusy If the ST-HAL returns a hal busy.
	 * @throws WriteReadHardware_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void writeReadHardware(const uint8_t writeData[], uint8_t readBuffer[], size_t size) override;
	/**
	 * @copydoc SpiSlaveHardware::stopReadHardware()
	 * @throws StopReadHardware_HalError If the ST-HAL returns a hal error.
	 * @throws StopReadHardware_HalBusy If the ST-HAL returns a hal busy.
	 * @throws StopReadHardware_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void stopReadHardware() override;
	/**
	 * @copydoc SpiSlaveHardware::stopWriteHardware()
	 * @throws StopWriteHardware_HalError If the ST-HAL returns a hal error.
	 * @throws StopWriteHardware_HalBusy If the ST-HAL returns a hal busy.
	 * @throws StopWriteHardware_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void stopWriteHardware() override;
	/**Emits the signal \c dataAvailable and resets the busy flag.*/
	void onReadWritten();
	/**Handles the read written ISR.*/
	void onReadWrittenHardware();
	/**Handles the abort ISR.*/
	void onAbortedHardware();
	void setFormatHardware(uint8_t bits, TransmissionMode transmission, WireMode wire) override;

private:
	/**Native hardware handle.*/
	SPI_HandleTypeDef* m_hwHandle;
	/**Queue containing all spi slaves.*/
	static LinkedQueue<Stm32SpiSlave> m_queue;
	/**Signal for handling the abort callback*/
	Signal<> aborted;
	/**Slot for aborted signal.*/
	SEMF_SLOT(m_abortedSlot, Signal<>, writeStopped, emitSignal);
	/**Current write data.*/
	uint8_t* m_writeData;
	/**Size of current write data.*/
	size_t m_writeDataSize;
	/**buffer for reading data*/
	uint8_t* m_readBuffer;
	/**Use DMA HAL functions.*/
	bool m_useDma;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32SpiSlave;
};
}  // namespace semf
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32_STM32SPISLAVE_H_ */
