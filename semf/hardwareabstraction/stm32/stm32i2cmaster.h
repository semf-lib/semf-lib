/**
 * @file stm32i2cmaster.h
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32_STM32I2CMASTER_H_
#define SEMF_HARDWAREABSTRACTION_STM32_STM32I2CMASTER_H_

#include <semf/communication/i2cmasterhardware.h>
#include <semf/hardwareabstraction/stm32/stm32.h>
#include <semf/utils/core/queues/linkedqueue.h>
#include <semf/utils/core/signals/slot.h>

#if defined(STM32) && defined(HAL_I2C_MODULE_ENABLED)
namespace semf
{
/**
 * @brief \c I2cMasterHardware implementation for STM32.
 */
class Stm32I2cMaster : public I2cMasterHardware, public LinkedQueue<Stm32I2cMaster>::Node
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
		SetFrequency_IsBusy,
		IsrError_HalError,
		ReadHardware_HalError,
		ReadHardware_HalBusy,
		ReadHardware_HalTimeout,
		WriteHardware_HalError,
		WriteHardware_HalBusy,
		WriteHardware_HalTimeout,
	};
	/**
	 * @brief Constructor.
	 * @param hwHandle i2c hardware interface
	 */
	explicit Stm32I2cMaster(I2C_HandleTypeDef& hwHandle);
	explicit Stm32I2cMaster(const Stm32I2cMaster& other) = delete;
	virtual ~Stm32I2cMaster() = default;

	/**
	 * @copydoc I2cMasterHardware::init()
	 * @throws Init_HalError If the ST-HAL returns a hal error.
	 * @throws Init_HalBusy If the ST-HAL returns a hal busy.
	 * @throws Init_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void init() override;
	/**
	 * @copydoc I2cMasterHardware::deinit()
	 * @throws Deinit_HalError If the ST-HAL returns a hal error.
	 * @throws Deinit_HalBusy If the ST-HAL returns a hal busy.
	 * @throws Deinit_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void deinit() override;
	/**
	 * @copydoc I2cMasterHardware::stopWrite()
	 * @throws StopWrite_HalError If the ST-HAL returns a hal error.
	 * @throws StopWrite_HalBusy If the ST-HAL returns a hal busy.
	 * @throws StopWrite_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void stopWrite() override;
	/**
	 * @copydoc I2cMasterHardware::stopRead()
	 * @throws StopRead_HalError If the ST-HAL returns a hal error.
	 * @throws StopRead_HalBusy If the ST-HAL returns a hal busy.
	 * @throws StopRead_HalTimeout If the ST-HAL returns a hal timeout.
	 */
	void stopRead() override;
	/**
	 * @copydoc I2c::setFrequency()
	 * @throws SetFrequency_IsBusy If this is busy.
	 */
	void setFrequency(uint32_t hz) override;
	void checkAddress(uint8_t address) override;
	/**
	 * @brief Get the list with all i2c.
	 * @return The list with all i2c.
	 */
	static LinkedQueue<Stm32I2cMaster>* queue();
	/**
	 * @brief System-wide interrupt service routine for i2c receive.
	 * @param i2c Hardware handler.
	 */
	static void systemIsrRead(I2C_HandleTypeDef& i2c);
	/**
	 * @brief System-wide interrupt service routine for i2c transmit.
	 * @param i2c Hardware handler.
	 */
	static void systemIsrWritten(I2C_HandleTypeDef& i2c);
	/**
	 * @brief System-wide interrupt service routine for i2c error.
	 * @param i2c Hardware handler.
	 */
	static void systemIsrError(I2C_HandleTypeDef& i2c);
	/**
	 * @brief System-wide interrupt service routine for process canceling.
	 * @param i2c Hardware handler.
	 */
	static void systemIsrAbort(I2C_HandleTypeDef& i2c);
	/**
	 * @brief Interrupt service routine for i2c object receive.
	 * @param i2c Hardware handler.
	 */
	void isrRead(I2C_HandleTypeDef& i2c);
	/**
	 * @brief Interrupt service routine for i2c object transmit.
	 * @param i2c Hardware handler.
	 */
	void isrWritten(I2C_HandleTypeDef& i2c);
	/**
	 * @brief Interrupt service routine for i2c object error.
	 * @param i2c Hardware handler.
	 * @throws IsrError_HalError If an unexpected error occures.
	 */
	void isrError(I2C_HandleTypeDef& i2c);
	/**
	 * @brief Interrupt service routine for i2c object abort.
	 * @param i2c Hardware handler.
	 */
	void isrAbort(I2C_HandleTypeDef& i2c);

protected:
	/**
	 * @brief Returns the hardware handle.
	 * @return hardware handle.
	 */
	I2C_HandleTypeDef* hardwareHandle();
	/**
	 * @brief Converts the frame information from semf to STM
	 * @param frame Frame information.
	 * @return STM frame information.
	 */
	virtual uint32_t hardwareFrame(CommunicationHardware::Frame frame) const;
	/**
	 * @copydoc I2cMasterHardware::readHardware()
	 * @throws ReadHardware_HalError If the ST-HAL returns a hal error.
	 * @throws ReadHardware_HalBusy If the ST-HAL returns a hal timeout.
	 * @throws ReadHardware_HalTimeout If the ST-HAL returns a hal busy.
	 */
	void readHardware(uint8_t buffer[], size_t bufferSize) override;
	/**
	 * @copydoc I2cMasterHardware::writeHardware()
	 * @throws WriteHardware_HalError If the ST-HAL returns a hal error.
	 * @throws WriteHardware_HalBusy If the ST-HAL returns a hal timeout.
	 * @throws WriteHardware_HalTimeout If the ST-HAL returns a hal busy.
	 */
	void writeHardware(const uint8_t data[], size_t dataSize) override;

private:
	/**
	 * @brief Slot for \checkAddress() function. Emit addressFound signal.
	 */
	void onAddressChecked();

	/**Will be emitted for read or write stopped.*/
	Signal<> canceled;
	/**Slot for canceled signal.*/
	SEMF_SLOT(m_canceledSlot, Signal<>, canceled, emitSignal);
	/**Slot for onAddressChecked function.*/
	SEMF_SLOT(m_onAddressCheckedSlot, Stm32I2cMaster, *this, onAddressChecked);
	/**Hardware handle*/
	I2C_HandleTypeDef* m_hwHandle;
	/**List for all i2c master.*/
	static LinkedQueue<Stm32I2cMaster> m_queue;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32I2cMaster;
};
} /* namespace semf */
#endif
#endif /* SEMF_HARDWAREABSTRACTION_STM32_STM32I2CMASTER_H_ */
