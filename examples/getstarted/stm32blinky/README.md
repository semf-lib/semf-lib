# STM32CubeIDE - semf - Blinky example

## General

In this tutorial, we will create a simple Blinky project using the **semf** library. It demonstrates the basic steps to set up a base project for the STM32F4 Discovery board and shows how to quickly and easily integrate and use the **semf** library. The complete finished project is located in this directory.

### Prerequisites

- **Controller**: STM32F4(29I-DISC1) Board
- **Development Environment**:
  - STM32CubeIDE (1.15.1)
  - STM32CubeMX (Plug-In Version 6.11.1-RC2)
  - PlantUML (1.2024.6)

### Steps Overview

- **Create the Project**: We start by creating a new project in STM32CubeIDE.
- **Integrate the semf Library**: Next, we integrate the semf library into our project.
- **Implement the Blinky Code**: Finally, we implement the code to make an LED on the STM32F4 Discovery board blink.

### How the Application Works

The application is based on the relaxed layer architecture (see `./doc/architecture.puml`) and uses the signal / slot principle. Its functionality is as follows:

- If the user button (blue) is not pressed, the green LED blinks.
- If the user button is pressed, the blinking is interrupted.

## Setup the Project

### Create a STM32CubeIDE C++ project

1. Start STM32CubeIDE
2. Create a New STM32 Project
3. Taget Selection &rarr; Board Selector &rarr; *e.g. STM32F429I-DISC1* &rarr; Next
4. Project Name: **stm32blinky**
5. Location: `<path to project>\stm32blinky`
6. Targeted Language: C++
7. Finish
8. Initialize all periphearals with their default Mode? **No**
9. Open the .ioc file
10. Generate the code by pressing the yellow gear wheel.

The project should build.

### Add the semf library (requires the source code)

#### Add the Lib-folder

1. Copy the semf repository (Lib folder) into the root directory of the project (`stm32blinky/Lib`)
2. Refresh the project

Alternatively (if the example is created with git, semf can also be inserted as a submodule). Just make sure that semf is specified correctly in the include paths.

#### Update include paths (semf)

1. Right click to Blinky project &rarr; Properties &rarr; C/C++ General &rarr; Paths and Symbols
   - Includes
     - GNU C
       - Add... `Lib`
     - GNU C++
        - Add... `Lib`
   - Source Location
     - Add folder... `/stm32blinky/Lib`
       - Filter: `test/`
       - Filter: `semf/test/`
2. Delete the examples folder `./Lib/examples`
3. Apply and close

#### Update compiler settings

1. Right click to project &rarr; Properties &rarr; C/C++ Build &rarr; Settings
   1. MCU GCC Compiler &rarr; General &rarr; GNU17
   2. MCU G++ Compiler &rarr; General &rarr; GNU++17
2. Apply and close

#### Make the SysTick counting

Finally, the SysTick must be linked to semf. Add the following code change to `stm32f4xx_it.c`:

```cpp
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
  HAL_SYSTICK_Callback();
  /* USER CODE END SysTick_IRQn 1 */
}
```

### C to C++ bridge

In order to be able to develop in C++ in the C++ converted project, a bridge must be inserted. Create a new folder in the project root: `./src`

#### ./src/maincpp.h (new file)

```cpp
#ifndef CORE_SRC_MAINCPP_H_
#define CORE_SRC_MAINCPP_H_

#ifdef __cplusplus
extern "C"
{
#endif

  void maincpp();

#ifdef __cplusplus
}
#endif

#endif  // CORE_SRC_MAINCPP_H_
```

#### ./src/maincpp.cpp (new file)

```cpp
#include <maincpp.h>

#include <Src/layers/msp.h>
#include <Src/layers/bsp.h>
#include <Src/layers/app.h>

void maincpp()
{
  // Create layer architecture
  static msp::Msp msp;
  static bsp::Bsp bsp(msp);
  static app::App app(bsp);

  while (1)
  {
    // do something
  }
}
```

#### ./src/main.c (changes)

```cpp
/* USER CODE BEGIN Includes */
#include <maincpp.h>
/* USER CODE END Includes */
```

```cpp
/* USER CODE BEGIN 2 */
maincpp();
/* USER CODE END 2 */
```

#### Update include paths (c to cpp bridge)

- Right click to Blinky project &rarr; Properties &rarr; C/C++ General &rarr; Paths and Symbols
  - Includes
    - GNU C
      - Add... `src`
    - GNU C++
      - Add... `src`
  - Source Location
    - Add Foler... `stm32blinky/src`

#### Integrate the blinky application

1. Copy the source code form `Lib/examples/getstarted/stm32blinky/src` to `.src/`
2. Create a debug configuration
3. Debug stm32blinky
