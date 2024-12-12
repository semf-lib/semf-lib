/**
 * @file stm32.h
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_HARDWAREABSTRACTION_STM32_STM32_H_
#define SEMF_HARDWAREABSTRACTION_STM32_STM32_H_

#if defined(STM32L053xx) | defined(STM32L052xx) | defined(STM32L073xx)
#define STM32
#define STM32L0
#include <stm32l0xx.h>
#endif

#if defined(STM32F030x6) || defined(STM32F030x8) || defined(STM32F031x6) || defined(STM32F038xx) || defined(STM32F042x6) || defined(STM32F048xx) || \
	defined(STM32F070x6) || defined(STM32F051x8) || defined(STM32F058xx) || defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx) || \
	defined(STM32F070xB) || defined(STM32F091xC) || defined(STM32F098xx) || defined(STM32F030xC)
#define STM32
#define STM32F0
#include <stm32f0xx.h>
#endif

#if defined(STM32G0B1xx) || defined(STM32G031xx)
#define STM32
#define STM32G0
#include <stm32g0xx.h>
#endif

#if defined(STM32G431xx)
#define STM32
#define STM32G4
#include <stm32g4xx.h>
#endif

#if defined(STM32F100xB) || defined(STM32F100xE) || defined(STM32F101x6) || defined(STM32F101xB) || defined(STM32F101xE) || defined(STM32F101xG) || \
	defined(STM32F102x6) || defined(STM32F102xB) || defined(STM32F103x6) || defined(STM32F103xB) || defined(STM32F103xE) || defined(STM32F103xG) || \
	defined(STM32F105xC) || defined(STM32F107xC)
#define STM32
#define STM32F1
#include <stm32f1xx.h>
#endif

#if defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx) || defined(STM32F302xC) || defined(STM32F303xC) || defined(STM32F358xx) || \
	defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx) || defined(STM32F302xE) || defined(STM32F303xE) || defined(STM32F398xx) || \
	defined(STM32F373xC) || defined(STM32F378xx)
#define STM32
#define STM32F3
#include <stm32f3xx.h>
#endif

#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx) || defined(STM32F427xx) || defined(STM32F437xx) || \
	defined(STM32F429xx) || defined(STM32F439xx) || defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F410Tx) || defined(STM32F410Cx) || \
	defined(STM32F410Rx) || defined(STM32F411xE) || defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Cx) || \
	defined(STM32F412Rx) || defined(STM32F412Vx) || defined(STM32F412Zx) || defined(STM32F413xx) || defined(STM32F423xx)
#define STM32
#define STM32F4
#include <stm32f4xx.h>
#endif

#if defined(STM32F756xx) || defined(STM32F746xx) || defined(STM32F745xx) || defined(STM32F767xx) || defined(STM32F769xx) || defined(STM32F777xx) || \
	defined(STM32F779xx) || defined(STM32F722xx) || defined(STM32F723xx) || defined(STM32F732xx) || defined(STM32F733xx) || defined(STM32F730xx) || \
	defined(STM32F750xx)
#define STM32
#define STM32F7
#include <stm32f7xx.h>
#endif

#if defined(STM32U5A5xx)
#define STM32
#define STM32U5
#include <stm32u5xx.h>
#endif

#endif /* SEMF_HARDWAREABSTRACTION_STM32_STM32_H_ */
