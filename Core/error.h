/**
 * @file error.h
 * @date 09.02.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef CORE_ERROR_H_
#define CORE_ERROR_H_

#include <cstdint>

namespace semf
{
/**
 * @brief Class for representing errors. Every error should have a unique source code. As a user feel encouraged to define your own errors and pass them via
 *        error signals and improve your debugging and error handling.
 */
class Error
{
public:
	/**
	 * @brief Semf class IDs.
	 */
	enum ClassID : uint32_t
	{
		AnalogDevicesAd5686 = 0,
		AnalogDevicesLtc1867,
		BoschBme280,
		CanHardware,
		EepromEmulation,
		ExternalInterruptHardware,
		FirmwareHeader,
		FirmwareUpdater,
		FimrwareVerifier,
		FlashLogger,
		FlashTester,
		FlashTesterRandom,
		FlashTesterReader,
		FlashTesterWriter,
		FlashVerifier,
		HammingCode,
		I2cEeprom,
		I2cMasterHardware,
		I2cScanner,
		I2cSlaveDevice,
		I2cSlaveHardware,
		I2cSlaveRegisterDevice,
		Logger,
		LoggerEntry,
		MicrochipSst26vf016b,
		MicrochipMcp3426,
		OnSemiLc709204F,
		PniRm3100i2c,
		Secded,
		SoftI2cMaster,
		SpiMasterHardware,
		SpiNorFlash,
		SpiSlaveDevice,
		SpiSlaveHardware,
		SpiSlaveRegisterDevice,
		StmLsm6Dso32I2c,
		StreamProtocol,
		TiAds1115,
		TiDrv425,
		UartHardware,

		SectionHardwareBegin = 0x08000000,

		SectionAnaloginBegin,
		Stm32AnalogIn,
		Stm32AnalogInDma,
		Stm32AnalogInInjected,
		Esp32S2AnalogIn,
		NetX90AnalogIn,
		NetX90AnalogInDma,
		SectionAnaloginEnd,

		SectionAnalogoutBegin,
		Stm32AnalogOut,
		Stm32AnalogOutDma,
		SectionAnalogoutEnd,

		SectionBootloaderBegin,
		SectionBootloaderEnd,

		SectionCanBegin,
		Stm32Can,
		SectionCanEnd,

		SectionCriticalSectionBegin,
		SectionCriticalSectionEnd,

		SectionExternalInterruptBegin,
		Stm32ExternalInterrupt,
		NetX90ExternalInterrupt,
		SectionExternalInterruptEnd,

		SectionFlashBegin,
		QtFlash,
		Stm32Flash,
		Esp32S2SpiFlash,
		SectionFlashEnd,

		SectionGpioBegin,
		NetX90Gpio,
		SectionGpioEnd,

		SectionI2cMasterBegin,
		Stm32F1I2cMaster,
		Stm32F3I2cMaster,
		Stm32F4I2cMaster,
		Stm32F7I2cMaster,
		Stm32I2cMaster,
		Stm32L0I2cMaster,
		Esp32S2I2cMaster,
		NetX90I2cMaster,
		SectionI2cMasterEnd,

		SectionI2cSlaveBegin,
		Stm32F1I2cSlave,
		Stm32F3I2cSlave,
		Stm32F4I2cSlave,
		Stm32F7I2cSlave,
		Stm32I2cSlave,
		Stm32L0I2cSlave,
		SectionI2cSlaveEnd,

		SectionInputCaptureBegin,
		Stm32InputCapture,
		SectionInputCaptureEnd,

		SectionPowerBegin,
		SectionPowerEnd,

		SectionPwmBegin,
		Stm32Pwm,
		Esp32S2LedControlPwm,
		NetX90Pwm,
		SectionPwmEnd,

		SectionRtcBegin,
		Stm32Rtc,
		SectionRtcEnd,

		SectionSpiMasterBegin,
		Stm32SpiMaster,
		NetX90SpiMaster,
		SectionSpiMasterEnd,

		SectionSpiSlaveBegin,
		Stm32SpiSlave,
		SectionSpiSlaveEnd,

		SectionTimerBegin,
		Stm32Timer,
		Esp32S2Timer,
		Esp32S2TimerPeripheral,
		NetX90Timer,
		SectionTimerEnd,

		SectionOutputCompareBegin,
		Stm32OutputCompare,
		SectionOutputCompareEnd,

		SectionUartBegin,
		Stm32Uart,
		Esp32S2Uart,
		QtUart,
		NetX90Uart,
		SectionUartEnd,

		SectionUsbVcpBegin,
		Stm32F4UsbVcp,
		Stm32F7UsbVcp,
		Stm32G0UsbVcp,
		SectionUsbVcpEnd,

		SectionHardwareEnd = 0x0FFFFFFF,

		SectionUserBegin = 0x10000000,
		SectionUserEnd = 0xFFFFFFFF
	};
	/**
	 * @brief Constructor.
	 * @param classId A class Id for identifying the class submitting the error (poor man's RTTI).
	 * @param errorCode Code for Identifying the source code location inside the failing class.
	 */
	Error(uint32_t classId, uint8_t errorCode);
	/**
	 * @brief Checks if the error got submitted by a class from the semf-HAL.
	 * @return True if error is from the semf-HAL, false otherwise.
	 */
	bool isHardwareError() const;
	/**
	 * @brief Returns the class id from the error.
	 * @return A class Id for identifying the class submitting the error (poor man's RTTI).
	 */
	uint32_t classId() const;
	/**
	 * @brief Returns the error code from the error.
	 * @return Code for Identifying the source code location inside the failing class.
	 */
	uint8_t errorCode() const;

private:
	/**Class Id.*/
	uint32_t m_classId;
	/**Error code.*/
	uint8_t m_errorCode;
};

}  // namespace semf
#endif  // CORE_ERROR_H_
