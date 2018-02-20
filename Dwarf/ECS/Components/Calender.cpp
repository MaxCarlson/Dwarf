#include "stdafx.h"
#include "Calender.h"

namespace TicksPer // Simulation ticks per game time
{
	constexpr int minute = 40;
	constexpr int hour   = 2500;
	constexpr int day    = 60000;
	constexpr int month  = 900000;
	constexpr int year   = 3600000;
}

namespace TimePer // Game time to Real Seconds
{
	constexpr double minute = 0.6667;
	constexpr double hour   = 41.0116;
	constexpr double day    = 984.2788;
	constexpr double month  = 14764.1832;
	constexpr double year   = 59056.7327;
}


void Calender::nextMinute()
{
	minute += 0.025;

	if (minute > 60.0)
	{
		++hour;
		minute = 0.0;
	}
	if (hour > 23)
	{
		++day;
		hour = 0;
	}
	if (day > 15)
	{
		++month;
		day = 0;
	}
	if (month > 11)
	{
		++year;
		month = 0;
	}
}

std::string Calender::getTime()
{
	std::string time = std::to_string(hour) + ':';

	time += std::to_string(static_cast<int>(minute)) + ':';
	time += "00  ";

	time += std::to_string(day + 1) + '/';
	time += std::to_string(month + 1) + '/';
	time += std::to_string(year) + '/';

	return time;
}
