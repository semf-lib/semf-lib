/**
 * @file stm32uart.h
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32_STM32UART_H_
#define SEMF_HARDWAREABSTRACTION_STM32_STM32UART_H_

#include <semf/hardwareabstraction/stm32/stm32.h>
#include <semf/utils/core/queues/linkedqueue.h>

#if defined(STM32) && defined(HAL_UART_MODULE_ENABLED)
#include <semf/communication/uarthardware.h>
namespace semf
{
/**
 * @brief \c UartHardware implemenation for STM32.
 */
class Stm32Uart : public UartHardware, public LinkedQueue<Stm32Uart>::Node
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
		StopWrite_HalError,
		StopWrite_HalBusy,
		StopWrite_HalTimeout,
		StopRead_HalError,
		StopRead_HalBusy,
		StopRead_HalTimeout,
		SetFormat_WordLengthInvalid,
		SetFormat_WireInvalid,
		SetFormat_ParityInvalid,
		SetFormat_StopBitsInvalid,
		SetFormat_FlowInvalid,
		WriteHardware_HalError,
		WriteHardware_HalBusy,
		WriteHardware_HalTimeout,
		ReadHardware_HalError,
		ReadHardware_HalBusy,
		ReadHardware_HalTimeout
	};

	/**
	 * @brief Constructor.
	 * @param hwHandle uart hardware interface
	 */
	explicit Stm32Uart(UART_HandleTypeDef& hwHandle);
	explicit Stm32Uart(const Stm32Uart& other) = delete;
	virtual ~Stm32Uart() = default;

	/**
	 * @copydoc UartHardware::init()
	 * @throws Init_HalError If the ST-HAL returns a hal error.
	 * @throws Init_HalBusy If the ST-HAL returns a hal bussy.
	 * @throws Init_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void init() override;
	/**
	 * @copydoc UartHardware::deinit()
	 * @throws Deinit_HalError If the ST-HAL returns a hal error.
	 * @throws Deinit_HalBusy If the ST-HAL returns a hal bussy.
	 * @throws Deinit_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void deinit() override;
	/**
	 * @copydoc UartHardware::stopWrite()
	 * @throws StopWrite_HalError If the ST-HAL returns a hal error.
	 * @throws StopWrite_HalBusy If the ST-HAL returns a hal bussy.
	 * @throws StopWrite_HalTimeout If the ST-HAL returns a hal timeout.
	 * @note This implementation is blocking and emits \c writeStopped without stack overflow protection
	 */
	void stopWrite() override;
	/**
	 * @copydoc UartHardware::stopRead()
	 * @throws StopRead_HalError If the ST-HAL returns a hal error.
	 * @throws StopRead_HalBusy If the ST-HAL returns a hal bussy.
	 * @throws StopRead_HalTimeout If the ST-HAL returns a hal timeout.
	 * @note This implementation is blocking and emits \c readStopped without stack overflow protection
	 */
	void stopRead() override;
	/**
	 * @copydoc UartHardware::setFormat()
	 * @throws SetFormat_WordLengthInvalid If bits is invalid (7, 8 and 9 are possible).
	 * @throws SetFormat_WireInvalid If wire is invalid.
	 * @throws SetFormat_ParityInvalid If par is invalid.
	 * @throws SetFormat_StopBitsInvalid If stop is invalid.
	 * @throws SetFormat_FlowInvalid If flow is invalid.
	 */
	void setFormat(uint8_t bits, WireMode wire, Parity par, StopBits stop, FlowControl flow) override;
	void setBaud(uint32_t baud) override;
	uint32_t baud() override;
	/**
	 * @brief Get the list with all uarts.
	 * @return The list with all uarts.
	 */
	static LinkedQueue<Stm32Uart>* queue();
	/**
	 * @brief System-wide interrupt service routine for uart receive.
	 * @param uart Hardware handler.
	 */
	static void systemIsrRead(UART_HandleTypeDef& uart);
	/**
	 * @brief System-wide interrupt service routine for uart transmit.
	 * @param uart Hardware handler.
	 */
	static void systemIsrWritten(UART_HandleTypeDef& uart);
	/**
	 * @brief Interrupt service routine for uart object receive.
	 * @param uart Hardware handler.
	 */
	void isrRead(UART_HandleTypeDef& uart);
	/**
	 * @brief Interrupt service routine for uart object transmit.
	 * @param uart Hardware handler.
	 */
	void isrWritten(UART_HandleTypeDef& uart);

protected:
	/**
	 * @copydoc UartHardware::writeHardware()
	 * @throws WriteHardware_HalError If the ST-HAL returns a hal error.
	 * @throws WriteHardware_HalBusy If the ST-HAL returns a hal bussy.
	 * @throws WriteHardware_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void writeHardware(const uint8_t data[], size_t dataSize) override;
	/**
	 * @copydoc UartHardware::readHardware()
	 * @throws ReadHardware_HalError If the ST-HAL returns a hal error.
	 * @throws ReadHardware_HalBusy If the ST-HAL returns a hal bussy.
	 * @throws ReadHardware_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void readHardware(uint8_t buffer[], size_t bufferSize) override;

private:
	/**
	 * @brief Locks the hardware.
	 * @note Function is necessary, because of a possible return value in STM LOCK function.
	 * @return busy for is already locked.
	 */
	HAL_StatusTypeDef halLock();

	/**List for all uarts*/
	static LinkedQueue<Stm32Uart> m_queue;
	/**Hardware handler*/
	UART_HandleTypeDef* m_hwHandle;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32Uart;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32_STM32UART_H_ */
