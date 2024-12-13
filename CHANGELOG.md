# Changelog
## v24.12.0
* Added STM32U5 `AnalogOut`, `Flash`, `Gpio`, `AnalogIn`, `AnalogInDma`, `Uart`, `Pwm`, `CriticalSection`, `SysTick`, `ExternalInterrupt`
* Refactored `Stm32OneWireMasterUart` to a generic `OneWireMasterUart`
* Bugfix for using crc in `FirmwareVerifier`

## v24.10.0
* Added `Stm32SpiMasterDma` and `Stm32SpiSlaveDma`
* Added `Encoder` abstraction
* Added `Stm32Encoder` and `Stm32G4Encoder`
* Added dependency to stm32, add baremetall guard, pull cmake to 3.24
* Added specific channel update in HardwareSensor
* Default and delete constructors, where missing
* Bugfix GoogleTest is only added on COMPILE_TESTS active
* Bugfix OneWireMaster depends now on active UART module

## v24.8.0
* Added example for `Sensor`, `EepromEmulation`, `AnalogOut`, `Timer`, `Errorhandling`, `Clock`, `Buffer`, `Crc`, `Errorhandling`, `Buffer`, `Crc`
* Added `Esp32AnalogIn` using the adc_continuous-API
* Added `Esp32SpiMaster`
* Saves RTC pointer in `Clock` as reference
* Bugfix for erasing elements of a `LinkedList`
* Bugfix for getting the correct element from `RingBuffer`

## v24.7.0
* Added Stm32G4 define into stm32 header
* Added `Stm32G4AnalogIn`, `Stm32G4Pwm`, `Stm32G4ExternalInterrupt`, `Stm32G4CriticalSSection`, `Stm32G4Timer`, `Stm32G4Systick`, `Stm32G4Gpio`, `Stm32G4Uart`, `Stm32G4I2cMaster`, `Stm32G4SpiMaster`
* Added `Uart` SingleWire-Support
* Added `OneWire`, `OneWireMaster` and `Stm32OneWireMaster`
* Updated ESP-HAL for IDF v5.2.2
* TX-FIFO size for `Esp32Uart` configurable
* BugFix Stm32 example
* BugFix stmlsm6dso32i2c registers data size
* BugFix DigitalOut toggle was not possible with inverted on

## v24.6.0
* Added `Stm32G0I2cSlave`
* Documentation for Stm32 example

## v24.5.0
* Added `Stm32G0I2cMaster`
* Optimized gitlab-release job
* Fixed missing setBusyWriting/Reading in Stm32Uart module

## v24.4.2
* Added IAR flag für little endian in std workarrounds
* Corrected object names in stm32 get started guide

## v24.4.1
* No auto deploy github, pipeline creates release repository

## v24.4.0
* Added github push test in pipeline
* Pipeline improvements
* Changed image for doc-builds

## v24.3.0
* Added keyword `virtual` to Buffer::count()
* Added `esh`
* Removed `std::iterator`
* Fixed AnalogDmaSensor buffer handing over
* Fixed SEMF_SIGNAL special case
* Fixed end-ConstIterators of LinkedListv
* Fixed Compile-error in Stm32Flash
* Removed duplicate signals from DigitalIn

## v24.1.0
* Added file structure and concepts page to documentation
* Fixed missing signal connection in SensorConverter

## v23.10.0
* Fixed various bugs of the LinkedList

## v23.9.0
* Fixed missing public inheritance of TimeBase

## v23.7.0
* Esp32Uart supports reading on high baudrates

## v23.6.0
* semf root directory
* Folders in lowercase letters
* App folder with new application interfaces introduced
* Utils folder introduced
* Unified styleguide
* Delete copy constructors introduced
* Virtual destructors introduced

## v23.5.0
* New slot implementation for flexible connections between signals and slots
* Override keyword introduced
* Stm32Can Warning for messages longer than 8 bytes
* LinkedQueue threw a Hardfault when queue.end() was called on an empty queue
* Stm32Flash constructor bugfix
* New sensor system for more flexibility in layer architectures
* Motion Tracking IC STM LSM6DSO32 I2C inherits from SensorHardware

## v23.4.0
* Building with -WConversion

## v23.3.0
* Added Stm32F0SpiMaster

## v23.2.0
* CI Improvements for STM32F0
* Added Stm32F0ExternalInterrupt
* Esp32CAN Driver added to HardwareAbstraction

## v22.11.0
* CD-Deploy Improvements
* Renamed Esp32S2 Hal to Esp32 Hal
* Removed pin lookup table from Esp32S2Uart and added PinConfiguration
* Readme editorial changes

## v22.10.0
* ESP-HAL unnecessary pointers removed
* Stm32G0 HAL added
* Interface for Modbus function handling added
* FreeModbus include paths corrected
* Stm32F0-Flash offset added

## v22.7.0
* Added FreeModbus with universal port layer
* ESP32-GPIO get pinnumber failsave
* fromLittleEndian corrected
* Motion Tracking IC STM LSM6DSO32 I2C version added
* Implemented automatic release job for gitlab
* Hardwaretester will be startet manually
* ESP32-ADC resolution can be configured
* Fuel-Gage IC OnSemi LC709204f added
* Stm32-External interrupt not used static queue removed
* FlashLogger emits cleared signal after memory was erased
* Flashtester deleted function restored
* SoftPwm fixed pwm value fixed to min or max value

## v22.6.0
* Added ChangelogListener to CI

## v21.3.1
* Changed Enum Format from UPPER_CASE to CamelCase
* Added Esp32S2Uart

## v21.3.0
* Added protected set busy functions to hardware communication classes
* Added unlock HAL to Stm32 communication classes
* Added weekday function to Date class
* Added AnalogOut and AnalogOutDma for Stm32F4
* FirmwareUpdater and Bootloader can handle multiple binaries
* Folder structure changed (Components removed)
* Added writePosition() and readPosition() to DataStream class
* Changed data length type to uint32_t for longer messages
* Added I2cSlave base class and implementation for Stm32F4 and Stm32F7
* Added implementations for setAddress, stopRead and stopWrite functions in I2cMaster and I2cSlave modules for all Stm32 devices
* Added readStopped and writeStopped signals to communication interface

## v1.3.0
* Added Debug class
* Added LinkedList and LinkedQueue classes
* Added Date, Time, DateTime, SystemClock, TimeBase and SoftwareTimer classes
* Added Logger and FlashLogger classes
* Added DataStream class
