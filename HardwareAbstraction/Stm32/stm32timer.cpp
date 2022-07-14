/**
 * @file stm32timer.cpp
 *
 * @date 31.01.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <HardwareAbstraction/Stm32/stm32timer.h>
#include <Core/debug.h>

#if defined(STM32) && defined(HAL_TIM_MODULE_ENABLED)
extern "C"
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	semf::Stm32Timer::systemIsr(*htim);
}
namespace semf
{
LinkedQueue<Stm32Timer> Stm32Timer::m_queue;
Stm32Timer::Stm32Timer(TIM_HandleTypeDef& hwHandle)
	:m_hwHandle(&hwHandle)
{
	queue()->push(*this);
}

void Stm32Timer::start()
{
	auto errorCode = HAL_TIM_Base_Start_IT(m_hwHandle);
	if (errorCode != HAL_OK)
	{
		if (errorCode == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint32_t>(ErrorCode::Start_HalError)));
		}
		else if (errorCode == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint32_t>(ErrorCode::Start_HalBusy)));
		}
		else if (errorCode == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint32_t>(ErrorCode::Start_HalTimeout)));
		}
		return;
	}
}

void Stm32Timer::stop()
{
	auto errorCode = HAL_TIM_Base_Stop_IT(m_hwHandle);
	if (errorCode != HAL_OK)
	{
		if (errorCode == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint32_t>(ErrorCode::Stop_HalError)));
		}
		else if (errorCode == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint32_t>(ErrorCode::Stop_HalBusy)));
		}
		else if (errorCode == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint32_t>(ErrorCode::Stop_HalTimeout)));
		}
		return;
	}
}

void Stm32Timer::reset()
{
	m_hwHandle->Instance->CNT = m_hwHandle->Instance->ARR;
}

LinkedQueue<Stm32Timer>* Stm32Timer::queue()
{
	static LinkedQueue<Stm32Timer> queue;
	return &queue;
}

void Stm32Timer::systemIsr(TIM_HandleTypeDef& hwHandle)
{
	for (Stm32Timer& i : *(queue()))
		i.isr(hwHandle);
}

void Stm32Timer::isr(TIM_HandleTypeDef& hwHandle)
{
	if (&hwHandle == m_hwHandle)
	{
		SEMF_INFO("timeout");
		timeout();
	}
}

} /* namespace semf */

#endif
