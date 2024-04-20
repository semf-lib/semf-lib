/**
 * @file stm32i2ccallbacks.cpp
 * @date 20.01.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/hardwareabstraction/stm32/stm32i2cslave.h>
#include <semf/hardwareabstraction/stm32/stm32i2cmaster.h>

#if defined(STM32) && defined(HAL_I2C_MODULE_ENABLED)
extern "C"
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c)
{
	semf::Stm32I2cMaster::systemIsrRead(*hi2c);
}
extern "C"
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* hi2c)
{
	semf::Stm32I2cMaster::systemIsrWritten(*hi2c);
}
extern "C"
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef* hi2c)
{
	semf::Stm32I2cSlave::systemIsrRead(*hi2c);
}
extern "C"
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef* hi2c)
{
	semf::Stm32I2cSlave::systemIsrWritten(*hi2c);
}
extern "C"
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef* hi2c)
{
	semf::Stm32I2cSlave::systemIsrListen(*hi2c);
}
extern "C"
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef* hi2c)
{
	semf::Stm32I2cMaster::systemIsrError(*hi2c);
	semf::Stm32I2cSlave::systemIsrError(*hi2c);
}
extern "C"
void HAL_I2C_AddrCallback(I2C_HandleTypeDef* hi2c, uint8_t direction, uint16_t addrMatchCode)
{
	semf::Stm32I2cSlave::systemIsrAddressing(*hi2c, direction, addrMatchCode);
}
extern "C"
void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef* hi2c)
{
	semf::Stm32I2cMaster::systemIsrAbort(*hi2c);
}
#endif
