# Sensor Example

## General
This example shows how the **semf** sensor module works. It demonstrates the steps from an digitized value, read by the ADC, to an converted value, printed by the application.

## Prerequisites
* Target:
  * Windows or Linux Desktop
* Development Environment:
  * Visual Studio Code or QT-Creator
* Enable CMAKE flag `COMPILE_EXAMPLES`
* build target `sensor`

## How the Application Works
The application simulates two analog sensors: one for measuring temperature and the other for monitoring humidity.

The hardware sensors are represented by two `VirtualAnalogIn` objects, with their measurement values being cyclically adjusted within the while-loop in the `AnalogInManipulator` class. After the adjustments, the `dataAvailable` signal of the `VirtualAnalogIn` objects is triggered by invoking the `start()` functions of both `AnalogIn` objects.

The interface between `msp` and `bsp` is managed through two `semf::AnalogIn` references, while the connection between `bsp` and `app` is managed via two classes interiting from `semf::app::Sensor` interface.

For converting the humidity sensor's values, the `SimpleSensorConverter` class is utilized. This provides a straightforward approach to implement a conversion function and represent measurement values within the application. On the other hand, the temperature sensor’s value conversion is handled by a custom implementation — the `Temperature` class within the bsp layer. This approach is typically applied for more complex sensor implementations, such as when calibration involving a factor and offset is required.

In the `MyApp` class, the `dataAvailable` signals from both sensors are connected to slots, which are responsible for printing the converted measurement values.