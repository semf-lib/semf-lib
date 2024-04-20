/**
 * @file stm32rtc.cpp
 * @date 21.12.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/hardwareabstraction/stm32/stm32rtc.h>
#include <semf/utils/core/debug.h>

#if defined(STM32) && defined(HAL_RTC_MODULE_ENABLED)
namespace semf
{
Stm32Rtc::Stm32Rtc(RTC_HandleTypeDef& hrtc)
: m_hrtc(&hrtc)
{
}

uint16_t Stm32Rtc::millisecond()
{
#if defined(STM32F1)
	return 0;
#else
	auto time = halTime();
	return static_cast<uint16_t>(1000 * (time.SecondFraction - time.SubSeconds) / (time.SecondFraction + 1));
#endif
}

void Stm32Rtc::setMillisecond(uint16_t millisecond)
{
	SEMF_INFO("set milisecond %u", millisecond);
	RTC_TimeTypeDef hal = halTime();
#if !defined(STM32F0) && !defined(STM32F1)
	hal.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	hal.StoreOperation = RTC_STOREOPERATION_RESET;
#endif
	millisecond %= 1000;
#if !defined(STM32F1)
	hal.SubSeconds = hal.SecondFraction - (hal.SecondFraction + 1) * millisecond / 1000;
#endif
	HAL_StatusTypeDef state = HAL_RTC_SetTime(m_hrtc, &hal, RTC_FORMAT_BIN);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetMillisecond_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetMillisecond_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetMillisecond_HalTimeout)));
		}
		return;
	}
}

uint8_t Stm32Rtc::second() const
{
	return halTime().Seconds;
}

void Stm32Rtc::setSecond(uint8_t second)
{
	SEMF_INFO("set second %u", second);
	RTC_TimeTypeDef hal = halTime();
#if !defined(STM32F0) && !defined(STM32F1)
	hal.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	hal.StoreOperation = RTC_STOREOPERATION_RESET;
#endif
	hal.Seconds = second % 60;
	HAL_StatusTypeDef state = HAL_RTC_SetTime(m_hrtc, &hal, RTC_FORMAT_BIN);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetSecond_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetSecond_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetSecond_HalTimeout)));
		}
		return;
	}
}

uint8_t Stm32Rtc::minute() const
{
	return halTime().Minutes;
}

void Stm32Rtc::setMinute(uint8_t minute)
{
	SEMF_INFO("set minute %u", minute);
	RTC_TimeTypeDef hal = halTime();
#if !defined(STM32F0) && !defined(STM32F1)
	hal.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	hal.StoreOperation = RTC_STOREOPERATION_RESET;
#endif
	hal.Minutes = minute % 60;
	HAL_StatusTypeDef state = HAL_RTC_SetTime(m_hrtc, &hal, RTC_FORMAT_BIN);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetMinute_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetMinute_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetMinute_HalTimeout)));
		}
		return;
	}
}

uint8_t Stm32Rtc::hour() const
{
	return halTime().Hours;
}

void Stm32Rtc::setHour(uint8_t hour)
{
	SEMF_INFO("set hour %u", hour);
	RTC_TimeTypeDef hal = halTime();
#if !defined(STM32F0) && !defined(STM32F1)
	hal.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	hal.StoreOperation = RTC_STOREOPERATION_RESET;
#endif
	hal.Hours = hour;
	HAL_StatusTypeDef state = HAL_RTC_SetTime(m_hrtc, &hal, RTC_FORMAT_BIN);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetHour_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetHour_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetHour_HalTimeout)));
		}
		return;
	}
}

uint8_t Stm32Rtc::day() const
{
	return halDate().Date;
}

void Stm32Rtc::setDay(uint8_t day)
{
	SEMF_INFO("set day %u", day);
	RTC_DateTypeDef hal = halDate();
	hal.Date = day;
	HAL_StatusTypeDef state = HAL_RTC_SetDate(m_hrtc, &hal, RTC_FORMAT_BIN);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetDay_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetDay_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetDay_HalTimeout)));
		}
		return;
	}
}

uint8_t Stm32Rtc::month() const
{
	return halDate().Month;
}

void Stm32Rtc::setMonth(uint8_t month)
{
	SEMF_INFO("set month %u", month);
	RTC_DateTypeDef hal = halDate();
	hal.Month = month;
	HAL_StatusTypeDef state = HAL_RTC_SetDate(m_hrtc, &hal, RTC_FORMAT_BIN);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetMonth_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetMonth_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetMonth_HalTimeout)));
		}
		return;
	}
}

uint16_t Stm32Rtc::year() const
{
	return halDate().Year + 2000;
}

void Stm32Rtc::setYear(uint16_t year)
{
	SEMF_INFO("set year %u", year);
	RTC_DateTypeDef hal = halDate();
	hal.Year = static_cast<uint8_t>(year % 100);
	HAL_StatusTypeDef state = HAL_RTC_SetDate(m_hrtc, &hal, RTC_FORMAT_BIN);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetYear_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetYear_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::SetYear_HalTimeout)));
		}
		return;
	}
}

RTC_TimeTypeDef Stm32Rtc::halTime() const
{
	RTC_TimeTypeDef halTime;
	HAL_StatusTypeDef state = HAL_RTC_GetTime(m_hrtc, &halTime, RTC_FORMAT_BIN);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::HalTime_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::HalTime_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::HalTime_HalTimeout)));
		}
	}
	return halTime;
}

RTC_DateTypeDef Stm32Rtc::halDate() const
{
	RTC_DateTypeDef halDate;
	HAL_StatusTypeDef state = HAL_RTC_GetDate(m_hrtc, &halDate, RTC_FORMAT_BIN);
	if (state != HAL_OK)
	{
		if (state == HAL_ERROR)
		{
			SEMF_ERROR("hal error");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::HalDate_HalError)));
		}
		else if (state == HAL_BUSY)
		{
			SEMF_ERROR("hal busy");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::HalDate_HalBusy)));
		}
		else if (state == HAL_TIMEOUT)
		{
			SEMF_ERROR("hal timeout");
			error(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::HalDate_HalTimeout)));
		}
	}
	return halDate;
}
} /* namespace semf */
#endif
