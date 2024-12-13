# Timing and TimeBase Example

## General
This example shows how timing works in **semf**. It demonstrates how hardware \ref semf::app::Timer are abstracted by \ref semf::app::TimeBase and used by \ref semf::SoftwareTimer. It is also shown how non atomic accesses are secured by \ref semf::CriticalSection.

## Prerequisites
* Target:
  * Windows or Linux Desktop
* Development Environment:
  * Visual Studio Code or QT-Creator
* Enable CMAKE flag `COMPILE_EXAMPLES`
* build target `timebase`

## How the Application Works
This examples shows how hardware `Timer`, `TimeBase` and `SoftwareTimer` work together.

In the following diagram the realized timing solution is shown.

![Timing Example with CriticalSection](../../../semf/doc/references/timerandtimebase.png)

The `Timer` and `AnalogOut` objects are located in the `Msp` layer, while the `TimeBase` objects resides in the `Bsp` layer.

The `ValueX` class ensures memory integrity by utilizing a `CriticalSection` for write access.

In the `MyApp` class, the calculations and printing operations are handled.