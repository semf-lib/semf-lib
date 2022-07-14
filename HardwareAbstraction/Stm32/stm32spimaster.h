/**
 * @file stm32spimaster.cpp
 *
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32_STM32SPIMASTER_H_
#define HARDWAREABSTRACTION_STM32_STM32SPIMASTER_H_

#include <HardwareAbstraction/Stm32/stm32.h>
#include <Core/Queues/linkedqueue.h>

#if defined(STM32) && defined(HAL_SPI_MODULE_ENABLED)
#include <Interfaces/spimasterhardware.h>
namespace semf
{
/**
 * @brief Class for using the spi in master mode.
 */
class Stm32SpiMaster : public SpiMasterHardware, public LinkedQueue<Stm32SpiMaster>::Node
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
		SetFrequency_IsBusy,
		SetFrequency_SpiClockIsZero,
		IsrError_Isr,
		SetFormatHardware_BitsNotSupported,
		StopWriteHardware_HalError,
		StopWriteHardware_HalBusy,
		StopWriteHardware_HalTimeout,
		StopReadHardware_HalError,
		StopReadHardware_HalBusy,
		StopReadHardware_HalTimeout,
		Write_HalError,
		Write_HalBusy,
		Write_HalTimeout,
		ReadNext_HalError,
		ReadNext_HalBusy,
		ReadNext_HalTimeout,
		WriteReadNext_HalError,
		WriteReadNext_HalBusy,
		WriteReadNext_HalTimeout,
	};
	/**
	 * @brief Constructor.
	 * @param spiHandle Hardware spi handle.
	 * @param spiClockFrequency Frequency of the clock running the SPI module in Hz.
	 */
	explicit Stm32SpiMaster(SPI_HandleTypeDef& hwHandle, uint32_t spiClockFrequencyHz = 0);

	/**
	 * @brief Constructor
	 * @param hwHandle SPI hardware interface.
	 * @param chipSelectPin	Gpio for chip select.
	 * @param spiClockFrequency Frequency of the clock running the SPI module in Hz.
	 */
	Stm32SpiMaster(SPI_HandleTypeDef& hwHandle, Gpio& chipSelectPin, uint32_t spiClockFrequencyHz = 0);
	/**
	 * @copydoc SpiMasterHardware::init()
	 * @throws Init_HalError If the ST-HAL returns a hal error.
	 * @throws Init_HalBusy If the ST-HAL returns a hal busy.
	 * @throws Init_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void init();
	/**
	 * @copydoc SpiMasterHardware::deinit()
	 * @throws Deinit_HalError If the ST-HAL returns a hal error.
	 * @throws Deinit_HalBusy If the ST-HAL returns a hal busy.
	 * @throws Deinit_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void deinit();
	/**
	 * @copydoc Spi::setFrequency()
	 * @throws SetFrequency_IsBusy If this is busy.
	 * @throws SetFrequency_SpiClockIsZero If spiClockFrequencyHz is not initialized.
	 */
	void setFrequency(uint32_t hz);

	/**
	 * @brief Get the list with all spi.
	 * @return The list with all spi.
	 */
	static LinkedQueue<Stm32SpiMaster>* queue();

	/**
	 * @brief System-wide interrupt service routine for spi receive.
	 * @param spi Hardware handler.
	 */
	static void systemIsrRead(SPI_HandleTypeDef& spi);

	/**
	 * @brief System-wide interrupt service routine for spi transmit.
	 * @param spi Hardware handler.
	 */
	static void systemIsrWritten(SPI_HandleTypeDef& spi);

	/**
	 * @brief System-wide interrupt service routine for spi receive and transmit.
	 * @param spi Hardware handler.
	 */
	static void systemIsrWrittenAndRead(SPI_HandleTypeDef& spi);

	/**
	 * @brief System-wide interrupt service routine for spi error.
	 * @param spi Hardware handler.
	 */
	static void systemIsrError(SPI_HandleTypeDef& spi);
	/**
	 * @brief System-wide interrupt service routine for spi abort.
	 * @param spi Hardware handler.
	 */
	static void systemIsrAbort(SPI_HandleTypeDef& spi);

	/**
	 * @brief Interrupt service routine for spi object receive.
	 * @param spi Hardware handler.
	 */
	void isrRead(SPI_HandleTypeDef& spi);

	void isrWritten(SPI_HandleTypeDef& spi);

	/**
	 * @brief Interrupt service routine for spi object receive and transmit.
	 * @param spi Hardware handler.
	 */
	void isrWrittenAndRead(SPI_HandleTypeDef& spi);

	/**
	 * @brief Interrupt service routine for spi object error.
	 * @param spi Hardware handler.
	 * @throws IsrError_Isr If an unexpected error occurres.
	 */
	void isrError(SPI_HandleTypeDef& spi);
	/**
	 * @brief handles the abort callback
	 * @param spi Hardware handler.
	 */
	void isrAborted(SPI_HandleTypeDef& spi);

protected:
	/**
	 * @copydoc SpiMasterHardware::writeHardware()
	 * @throws Write_HalError If the ST-HAL returns a hal error.
	 * @throws Write_HalBusy If the ST-HAL returns a hal busy.
	 * @throws Write_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void writeHardware(const uint8_t data[], size_t dataSize);
	void readHardware(uint8_t buffer[], size_t bufferSize);
	void writeReadHardware(const uint8_t writeData[], uint8_t readBuffer[], size_t size);
	/**
	 * @copydoc SpiMasterHardware::setFormatHardware()
	 * @throws SetFormatHardware_BitsNotSupported If value for bit is not supported (8 and 16 are allowed).
	 */
	void setFormatHardware(uint8_t bits, TransmissionMode transmission, WireMode wire);
	/**
	 * @copydoc SpiMasterHardware::stopWriteHardware()
	 * @throws StopWriteHardware_HalError If the ST-HAL returns a hal error.
	 * @throws StopWriteHardware_HalBusy If the ST-HAL returns a hal busy.
	 * @throws StopWriteHardware_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void stopWriteHardware();
	/**
	 * @copydoc SpiMasterHardware::stopReadHardware()
	 * @throws StopReadHardware_HalError If the ST-HAL returns a hal error.
	 * @throws StopReadHardware_HalBusy If the ST-HAL returns a hal busy.
	 * @throws StopReadHardware_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void stopReadHardware();

private:
	/**
	 * @brief Read the next data.
	 * @throws ReadNext_HalError If the ST-HAL returns a hal error.
	 * @throws ReadNext_HalBusy If the ST-HAL returns a hal busy.
	 * @throws ReadNext_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void readNext();
	/**
	 * @brief Write and read the next data.
	 * @throws ReadWriteNext_HalError If the ST-HAL returns a hal error.
	 * @throws ReadWriteNext_HalBusy If the ST-HAL returns a hal busy.
	 * @throws ReadWriteNext_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void writeReadNext();

	/**Hardware spi handle*/
	SPI_HandleTypeDef* m_hwHandle;
	/**Frequency of the clock running the SPI module in Hz.*/
	const uint32_t m_spiClockFrequencyHz;
	/**Buffering write data.*/
	const uint8_t* m_writeData = nullptr;
	/**Buffering read data.*/
	uint8_t* m_readData = nullptr;
	/**Size of read data array.*/
	size_t m_dataSize = 0;
	/**Index of momentary read byte.*/
	size_t m_dataIndex = 0;
	/**Callback of abort irq.*/
	Signal<> aborted;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32SpiMaster;
};

} /* namespace semf */

#endif

#endif /* HARDWAREABSTRACTION_STM32_STM32SPIMASTER_H_ */
