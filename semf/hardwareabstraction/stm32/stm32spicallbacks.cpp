/**
 * @file stm32spicallbacks.cpp
 * @date 27.01.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in 
 *            the project root directory.
 */

#include <semf/hardwareabstraction/stm32/stm32spimaster.h>
#include <semf/hardwareabstraction/stm32/stm32spislave.h>

#if defined(STM32) && defined(HAL_SPI_MODULE_ENABLED)
extern "C"
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi)
{
	semf::Stm32SpiMaster::systemIsrRead(*hspi);
	semf::Stm32SpiSlave::systemIsrRead(*hspi);
}
extern "C"
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{
	semf::Stm32SpiMaster::systemIsrWritten(*hspi);
	semf::Stm32SpiSlave::systemIsrWritten(*hspi);
}
extern "C"
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef* hspi)
{
	semf::Stm32SpiMaster::systemIsrWrittenAndRead(*hspi);
	semf::Stm32SpiSlave::systemIsrWrittenAndRead(*hspi);
}
extern "C"
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef* hspi)
{
	semf::Stm32SpiMaster::systemIsrError(*hspi);
	semf::Stm32SpiSlave::systemIsrError(*hspi);
}
extern "C"
void HAL_SPI_AbortCpltCallback(SPI_HandleTypeDef* hspi)
{
	semf::Stm32SpiMaster::systemIsrAbort(*hspi);
	semf::Stm32SpiSlave::systemIsrAbort(*hspi);
}
#endif
