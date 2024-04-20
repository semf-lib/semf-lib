# semf - smart embedded framework
With the semf middleware library lot of helpful modules and interfaces stand ready for your embedded projects. Because of the platform independency of all components, the source code can be reused easily and projects can be ported effortlessly.

## Characterizion
* Real C++ object orientation with open class architecture
* Reusable source code
* Platform independence
* Easily testable source code
* Less debugging
* Solves the big and small hurdles in everyday developer work

## Value proposition
* Saving of valuable developer time and cost reduction
* Significant reduction in time to market
* Fewer complaints and therefore happier customers
* Happier, more productive developers who enjoy coding more

To start using semf, see the [Documentation](https://github.com/semf-lib/semf-doc/index.html).

## Licensing
semf is available for commercial and non commercial usage under the following licenses:

1. For commercial usage: querdenker engineering end user license agreement for semf library
2. For non commercial usage: “Commons Clause” License Condition v1.0 [i.e. Apache 2.0]

## semf Components and Structure
The following structure reflects the folder structure of **semf** library. The highlights are directly linked to the class or interface documentation.

### Battery

In here all interfaces and classes to implement a state of charge and state of health for batteries can be found (under construction).

### Bootloader

Contains all classes to verify and flash new firmware.

    semf::Bootloader (*)
    semf::FirmwareUpdater (*)

### Communication

Communicating with any hardware outside the microcontroller, with or without a protocol.

    semf::esh::Shell
    semf::I2cScanner
    semf::I2cSlaveDevice (*)
    semf::I2cSlaveRegisterDevice (*)
    semf::SoftI2cMaster
    semf::SpiSlaveDevice (*)
    semf::SpiSlaveRegisterDevice (*)
    semf::StreamProtocol (*)

### Core

In core all elementary semf components can be found, the library is built with.

    semf::StdArray
    semf::Debug
    semf::Error
    semf::LastInBuffer
    semf::LastInDmaBuffer
    semf::LinkedList
    semf::LinkedQueue
    semf::RingBuffer
    semf::Signal
    semf::Slot
    semf::StaticSlot

### Hardware Abstraction

Includes all hardware dependent drivers.

    STMicroelectronics STM32F4

    STMicroelectronics STM32F0, STM32F1, STM32F3, STM32F7, STM32G0, STM32H7, STM32L0 (*)
    Espressif ESP32, ESP32-S2, ESP32-C3, ESP32-S3 (*)
    Hilscher NETX90 (*)
    NXP LPC1100, LPC1300, LPC1700, LPC1800, LPC54, LPC55, K32L2, K32L3 (*)
    Giga Devices GD32F1, GD32F2, GD32F3, GD32F4 (*)

### Input

Collects all classes for handling information entering the microcontroller except communication.

    semf::DigitalInInt
    semf::DigitalInPolling
    semf::DebouncedDigitalInPolling
    semf::FrequencyIn
    semf::Sensor

### Interfaces

Here, all interfaces for implementing new microcontroller hardware or hardware emulators can be found. In application classes these interfaces are often used for handling multiple hardware implementations.

    semf::AesCbc (*)
    semf::AnalogIn
    semf::AnalogInDma
    semf::AnalogOut
    semf::AnalogOutDma
    semf::CanHardware
    semf::Crc
    semf::CriticalSection
    semf::ErrorCorrectionCode (*)
    semf::ExternalInterrupt
    semf::Flash
    semf::Gpio
    semf::Hash (*)
    semf::I2cMasterHardware
    semf::I2cSlaveHardware
    semf::InputCapture
    semf::OutputCompare
    semf::Power
    semf::Pwm
    semf::Rtc
    semf::SignaturePkcs1 (*)
    semf::SpiMasterHardware
    semf::SpiSlaveHardware
    semf::Storage
    semf::Timer
    semf::UartHardware

### Output

Collects all classes for handling information going out of the microcontroller except communication.

    semf::DigitalOut
    semf::PwmOut
    semf::LedBlinking
    semf::LedDimming
    semf::SoftPwm

### Processing

Classes for processing data without using any microcontroller hardware (except core and FPU).

    semf::Average
    semf::CrcSoftware
    semf::DataStream (*)
    semf::Endian (Big- and Little)
    semf::DivideWithRound
    semf::LinearInterpolator
    semf::PidController
    semf::Secded (Hamming code) (*)

### Storage

Everything which is necessary for storing data into any storage or reading it out.

    semf::EccFlashlogger (*)
    semf::EepromEmulation (*)
    semf::FlashVerifier (*)
    semf::FlashTester (*)
    semf::I2cEeprom (*)
    semf::SpiNorFlash (*)
    semf::Flashlogger (*)

### System

Collection of classes for storing and handling time information and organizing the timing structure.

    semf::Date
    semf::DateTime
    semf::SoftwareTimer
    semf::SystemClock
    semf::Time
    semf::TimeBase

(*) Module is only available in payed version (including the commercial usage).