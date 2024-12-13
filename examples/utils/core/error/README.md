# Errorhandling Example

## General
This example shows how error handling works in **semf**.

## Prerequisites
* Target:
  * Windows or Linux Desktop
* Development Environment:
  * Visual Studio Code or QT-Creator
* Enable CMAKE flag `COMPILE_EXAMPLES`
* build target `error`

## How the Application Works
This examples shows how the **semf** error handling can be used in a user application.

A file lists all application components, such as the 'Component' class in this example. **semf** provides predefined start and end values for unique class IDs, which are used here.

The 'Component' class defines its own 'ErrorCodes', 'classId', 'error' signal, and a single function to set a buffer.

In the 'MyApp' class, the 'error' signal of 'Component' is connected to the 'onError' slot.

When the application starts, it attempts to initialize the buffer with nullptr, which triggers the 'error' signal.
