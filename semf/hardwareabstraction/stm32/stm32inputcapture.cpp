/**
 * @file stm32inputcapture.cpp
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/hardwareabstraction/stm32/stm32inputcapture.h>
#include <semf/utils/core/debug.h>

#if defined(STM32) && defined(HAL_TIM_MODULE_ENABLED)
extern "C" void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim)
{
	semf::Stm32InputCapture::systemIsr(*htim);
}
namespace semf
{
LinkedQueue<Stm32InputCapture> Stm32InputCapture::m_queue;
Stm32InputCapture::Stm32InputCapture(TIM_HandleTypeDef& hwHandle, uint32_t channel, unsigned int ticksPerSecond)
: m_hwHandle(&hwHandle),
  m_channel(channel),
  m_ticksPerSecond{ticksPerSecond}
{
	m_queue.push(*this);
}

void Stm32InputCapture::start()
{
	SEMF_INFO("start");
	auto state = HAL_TIM_IC_Start_IT(m_hwHandle, m_channel);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Start_Error)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Start_Busy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Start_Timeout)));
		}
		return;
	}
}

void Stm32InputCapture::stop()
{
	SEMF_INFO("stop");
	auto state = HAL_TIM_IC_Stop_IT(m_hwHandle, m_channel);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Stop_Error)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Stop_Busy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Stop_Timeout)));
		}
		return;
	}
}

unsigned int Stm32InputCapture::ticks()
{
	uint32_t val = HAL_TIM_ReadCapturedValue(m_hwHandle, m_channel);
	SEMF_INFO("value %u on channel %u", val, m_channel);
	return val;
}

void Stm32InputCapture::setTrigger(Trigger trigger)
{
	if (m_hwHandle->Lock == HAL_LOCKED)
	{
		SEMF_ERROR("handle is locked");
		error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetTrigger_Busy)));
		return;
	}
	m_hwHandle->Lock = HAL_LOCKED;

	// get the capture compare enable register
	uint32_t ccer = m_hwHandle->Instance->CCER;

	// disable the capture compare mode
	m_hwHandle->Instance->CCER &= ~(1 << (m_channel / 2));

	switch (trigger)
	{
		case Trigger::RisingEdge:
			ccer &= ~(1 << (m_channel + 1));
			ccer &= ~(1 << (m_channel + 3));
			break;
		case Trigger::FallingEdge:
			ccer |= 1 << (m_channel + 1);
			ccer &= ~(1 << (m_channel + 3));
			break;
		case Trigger::RisingAndFallingEdge:
			ccer |= 1 << (m_channel + 1);
			ccer |= 1 << (m_channel + 3);
			break;
		default:
			SEMF_ERROR("invalid value for trigger: %u", static_cast<uint32_t>(trigger));
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetTrigger_TriggerInvalid)));
			return;
	}

	m_hwHandle->Instance->CCER = ccer;
	__HAL_UNLOCK(m_hwHandle);
}

void Stm32InputCapture::systemIsr(TIM_HandleTypeDef& hwHandle)
{
	for (auto& i : m_queue)
		i.isr(hwHandle);
}

void Stm32InputCapture::isr(TIM_HandleTypeDef& hwHandle)
{
	if (m_hwHandle == &hwHandle && hwHandle.Channel == m_channel)
	{
		SEMF_INFO("dataAvailable");
		dataAvailable();
	}
}

unsigned int Stm32InputCapture::ticksPerSecond() const
{
	return m_ticksPerSecond;
}

void Stm32InputCapture::setMaxTicks(unsigned int maxTicks)
{
	SEMF_INFO("set max ticks %u", maxTicks);
	m_hwHandle->Instance->ARR = maxTicks;
}

unsigned int Stm32InputCapture::maxTicks() const
{
	return m_hwHandle->Instance->ARR;
}
} /* namespace semf */
#endif
