/**
 * @file stm32encoder.cpp
 * @date 18.09.2024
 * @author vb
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/hardwareabstraction/stm32/stm32encoder.h>
#include <semf/utils/core/debug.h>

#if defined(STM32) && defined(HAL_TIM_MODULE_ENABLED)

namespace semf
{
LinkedQueue<Stm32Encoder> Stm32Encoder::m_queue;

Stm32Encoder::Stm32Encoder(TIM_HandleTypeDef& hwHandle)
: m_hwHandle{&hwHandle}
{
	m_queue.push(*this);
}

void Stm32Encoder::start()
{
	HAL_StatusTypeDef status = HAL_TIM_Encoder_Start_IT(m_hwHandle, TIM_CHANNEL_ALL);

	if (status != HAL_OK)
	{
		if (status == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Start_HalError)));
		}
		else if (status == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Start_HalBusy)));
		}
		else if (status == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Start_HalTimeout)));
		}
		else
		{
			SEMF_ERROR("Undefined error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Undefined)));
		}
	}
}

void Stm32Encoder::stop()
{
	HAL_StatusTypeDef status = HAL_TIM_Encoder_Stop_IT(m_hwHandle, TIM_CHANNEL_ALL);
	if (status != HAL_OK)
	{
		if (status == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Stop_HalError)));
		}
		else if (status == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Stop_HalBusy)));
		}
		else if (status == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Stop_HalTimeout)));
		}
		else
		{
			SEMF_ERROR("Undefined error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Undefined)));
		}
	}
}

uint32_t Stm32Encoder::position()
{
	return m_position;
}

Stm32Encoder::Direction Stm32Encoder::direction()
{
	return m_direction;
}

void Stm32Encoder::systemIsr(TIM_HandleTypeDef& hwHandle)
{
	for (Stm32Encoder& i : m_queue)
		i.isr(hwHandle);
}

void Stm32Encoder::isr(TIM_HandleTypeDef& hwHandle)
{
	if (&hwHandle == m_hwHandle)
	{
		const uint32_t value = __HAL_TIM_GET_COUNTER(m_hwHandle) / kTimerStepsProEncoderPosition;
		const Direction direction = static_cast<Direction>(__HAL_TIM_DIRECTION_STATUS(m_hwHandle));

		if (m_position != value)
		{
			m_position = value;
			positionChanged(m_position, direction);
		}

		if (direction != m_direction)
		{
			m_direction = direction;
			directionChanged(m_direction);
		}
	}
	return;
}
}  // namespace semf
#endif
