/**
 * @file stm32i2cslave.h
 * @date 20.01.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32_STM32I2CSLAVE_H_
#define SEMF_HARDWAREABSTRACTION_STM32_STM32I2CSLAVE_H_

#include <semf/communication/i2cslavehardware.h>
#include <semf/hardwareabstraction/stm32/stm32.h>
#include <semf/utils/core/queues/linkedqueue.h>

#if defined(STM32) && defined(HAL_I2C_MODULE_ENABLED)
namespace semf
{
/**
 * @brief \c I2cSlaveHardware implementation for STM32.
 */
class Stm32I2cSlave : public I2cSlaveHardware, public LinkedQueue<Stm32I2cSlave>::Node
{
public:
	/**Error IDs for this class. Error ID identify a unique error() / onError call (excluding transferring).*/
	enum class ErrorCode : uint8_t
	{
		Init_HalError = 0,
		Init_HalBusy,
		Init_HalTimeout,
		Deinit_HalError,
		Deinit_HalBusy,
		Deinit_HalTimeout,
		SystemIsrListen_EnableListenHalError,
		SystemIsrListen_EnableListenHalBusy,
		SystemIsrListen_EnableListenHalTimeout,
		SetFrequency_IsBusy,
		SystemIsrError_HalError,
		ReadHardware_HalError,
		ReadHardware_HalBusy,
		ReadHardware_HalTimeout,
		WriteHardware_HalError,
		WriteHardware_HalBusy,
		WriteHardware_HalTimeout,
		StartListeningHardware_HalError,
		StartListeningHardware_HalBusy,
		StartListeningHardware_HalTimeout,
		StopListeningHardware_HalError,
		StopListeningHardware_HalBusy,
		StopListeningHardware_HalTimeout
	};

	/**
	 * @brief Constructor
	 * @param hwHandle Hardware handle.
	 */
	explicit Stm32I2cSlave(I2C_HandleTypeDef& hwHandle);
	explicit Stm32I2cSlave(const Stm32I2cSlave& other) = delete;
	virtual ~Stm32I2cSlave() = default;

	/**
	 * @copydoc I2cSlaveHardware::init()
	 * @throws Init_HalError If the ST-HAL returns a hal error.
	 * @throws Init_HalBusy If the ST-HAL returns a hal busy.
	 * @throws Init_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void init() override;
	/**
	 * @copydoc I2cSlaveHardware::deinit()
	 * @throws Deinit_HalError If the ST-HAL returns a hal error.
	 * @throws Deinit_HalBusy If the ST-HAL returns a hal busy.
	 * @throws Deinit_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void deinit() override;
	void stopRead() override;
	void stopWrite() override;
	/**
	 * @copydoc I2c::setFrequency()
	 * @throws SetFrequency_IsBusy If this is busy.
	 */
	void setFrequency(uint32_t hz) override;
	/**
	 * @brief System-wide isr for data reception on an I2C-bus.
	 * @param hwHandle Native handle.
	 */
	static void systemIsrRead(I2C_HandleTypeDef& hwHandle);
	/**
	 * @brief System-wide isr for data transmission on an I2C-bus.
	 * @param hwHandle Native handle.
	 */
	static void systemIsrWritten(I2C_HandleTypeDef& hwHandle);
	/**
	 * @brief System-wide isr for end of listening on an I2C-bus.
	 * @param hwHandle Native handle.
	 * @throws SystemIsrListen_EnableListenHalError If the ST-HAL returns a hal error.
	 * @throws SystemIsrListen_EnableListenHalBusy If the ST-HAL returns a hal busy.
	 * @throws SystemIsrListen_EnableListenHalTimeout If the ST-HAL returns a hal timeout.
	 */
	static void systemIsrListen(I2C_HandleTypeDef& hwHandle);
	/*
	 * @brief System-wide isr for an error on an I2C-bus.
	 * @param hwHandle Native handle.
	 */
	static void systemIsrError(I2C_HandleTypeDef& hwHandle);
	/*
	 * @brief System-wide isr getting addressed on an I2C-bus.
	 * @param hwHandle Native handle.
	 * @param direction Direction of the data transmission.
	 * @param addrMatchCode Address match code.
	 */
	static void systemIsrAddressing(I2C_HandleTypeDef& hwHandle, uint8_t direction, uint16_t addrMatchCode);

protected:
	/**
	 * @copydoc I2cSlaveHardware::readHardware()
	 * @throws ReadHardware_HalError If the ST-HAL returns a hal error.
	 * @throws ReadHardware_HalBusy If the ST-HAL returns a hal busy.
	 * @throws ReadHardware_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void readHardware(uint8_t buffer[], size_t bufferSize) override;
	/**
	 * @copydoc I2cSlaveHardware::writeHardware()
	 * @throws WriteHardware_HalError If the ST-HAL returns a hal error.
	 * @throws WriteHardware_HalBusy If the ST-HAL returns a hal busy.
	 * @throws WriteHardware_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void writeHardware(const uint8_t data[], size_t dataSize) override;
	/**
	 * @copydoc I2cSlaveHardware::startListeningHardware()
	 * @throws StartListeningHardware_HalError If the ST-HAL returns a hal error.
	 * @throws StartListeningHardware_HalBusy If the ST-HAL returns a hal busy.
	 * @throws StartListeningHardware_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void startListeningHardware() override;
	/**
	 * @copydoc I2cSlaveHardware::stopListeningHardware()
	 * @throws StopListeningHardware_HalError If the ST-HAL returns a hal error.
	 * @throws StopListeningHardware_HalBusy If the ST-HAL returns a hal busy.
	 * @throws StopListeningHardware_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void stopListeningHardware() override;
	void setAddressHardware(uint8_t address) override;
	/**
	 * @brief Converts the frame information from semf to STM.
	 * @param frame Frame information.
	 * @return STM frame information.
	 */
	virtual uint32_t hardwareFrame(CommunicationHardware::Frame frame) const;
	/**
	 * @brief Returns the hardware Handle.
	 * @return Pointer to hardware handle.
	 */
	I2C_HandleTypeDef* hardwareHandle();

private:
	/**Native hardware handle.*/
	I2C_HandleTypeDef* m_hwHandle;
	/**Queue with all slaves.*/
	static LinkedQueue<Stm32I2cSlave> m_queue;
	/**Read buffer address.*/
	uint8_t* m_readBuffer;
	/**Size of the read buffer.*/
	size_t m_readBufferSize;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32I2cSlave;
};
} /* namespace semf */
#endif
#endif  // SEMF_HARDWAREABSTRACTION_STM32_STM32I2CSLAVE_H_
