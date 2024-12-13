# Crc Example

## General
This example shows how the **semf** crc software works. It demonstrates the initialization and processing values.

## Prerequisites
* Target:
  * Windows or Linux Desktop
* Development Environment:
  * Visual Studio Code or QT-Creator
* Enable CMAKE flag `COMPILE_EXAMPLES`
* build target `crc`

## How the Application Works
First, two \ref semf::CrcSoftware objects are created — one with custom template arguments and the other with standard template arguments.

After that, one CRC is calculated with a single function call and evaluated, while the second one is calculated slice by slice.