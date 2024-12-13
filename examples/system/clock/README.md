# Clock Example

## General
This example shows how the **semf** real time clock works. It demonstrates the initialization and working with date time objects.

## Prerequisites
* Target:
  * Windows or Linux Desktop
* Development Environment:
  * Visual Studio Code or QT-Creator
* Enable CMAKE flag `COMPILE_EXAMPLES`
* build target `clock`

## How the Application Works
First a \ref semf::Clock object is created which basis on a \ref semf::Rtc object.

Second the actual time stemp is set to the \ref semf::Clock.

While checking a time span, the current date and time relative to the previously set time is printed five times, with a two-second interval between each print.