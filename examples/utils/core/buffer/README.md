# Buffer & Average Example

## General
This example shows how the **semf** buffer implementations work. It demonstrates the initialization, writing and reading values.

## Prerequisites
* Target:
  * Windows or Linux Desktop
* Development Environment:
  * Visual Studio Code or QT-Creator
* Enable CMAKE flag `COMPILE_EXAMPLES`
* build target `buffer`

## How the Application Works
The application is structured in three paragraphs:

1. `RingBuffer` example
2. `LastInBuffer` example
3. `LastInDmaBuffer` example

All three examples are structured in the same way:

1. Object creation
2. Filling the buffer with data
3. Reading out some data

For each type of buffer, the `Average` class returns the average value of the data contained within the buffer.