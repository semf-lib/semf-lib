/**
 * @file main.cpp
 * @author fs
 * @date 12.08.2024
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved.
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/hardwareabstraction/virtual/virtualrtc.h>
#include <semf/system/clock.h>
#include <semf/utils/system/datetime.h>
#include <thread>
#include <iostream>

std::string dateTimeToString(semf::DateTime dateTime)
{
	std::tm time{};
	time.tm_year = dateTime.year() - 1900;
	time.tm_mon = dateTime.month() - 1;
	time.tm_mday = dateTime.day();
	time.tm_hour = dateTime.hour();
	time.tm_min = dateTime.minute();
	time.tm_sec = dateTime.second();
	return std::asctime(&time);
}

int main()
{
	semf::VirtualRtc rtc;
	semf::Clock clock(rtc);

	// Setting clock to June 1st 2009 10:20:30 and 0 milliseconds
	semf::DateTime timeStamp(0, 30, 20, 10, 1, semf::Date::Month::June, 2009);
	clock = timeStamp;
	std::cout << "Set to: " << dateTimeToString(timeStamp) << std::endl;

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "Now: " << dateTimeToString(clock.now()) << std::endl;

	timeStamp = clock.now();
	for (size_t i = 0; i < 5;)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		if (clock.now().timeSpan(timeStamp) == 2000)
		{
			timeStamp = clock.now();
			std::cout << "Now: " << dateTimeToString(timeStamp) << std::endl;
			i++;
		}
	}
	return 0;
}
