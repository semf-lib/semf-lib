# AnalogOut Example

## General
This example shows how the **semf** virtual eeprom works. It demonstrates the initialization, reading and writing values.

## Prerequisites
* Target:
  * Windows or Linux Desktop
* Development Environment:
  * Visual Studio Code or QT-Creator
* Enable CMAKE flag `COMPILE_EXAMPLES`
* build target `analogout`

## How the Application Works
This example demonstrates how the `AnalogOut` and `AnalogOutDma` interfaces work by using the implementations `VirtualAnalogOut` and `VirtualAnalogOutDma`.

First, both objects are created.

For `AnalogOut`, the process is straightforward: a value is set by calling `setValue()` function, it is started by calling `start()`, and then stopped by calling the `stop()` functions.

The timing behavior of `VirtualAnalogOutDma` is configured by the constructor. After assigning a data array to the `AnalogOutDma` it is started, and the `AnalogOutDma` values are read at the same frequency specified by the constructor.

Once all the data has been output, the `dataWritten` signal is emitted by the `AnalogOutDma` class, triggering the `onAnalogOutDmaDataWritten()` slot.