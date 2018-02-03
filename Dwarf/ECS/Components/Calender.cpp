#include "stdafx.h"
#include "Calender.h"




void Calender::nextMinute()
{
	++minute;

	if (minute > 59)
	{
		++hour;
		minute = 0;
	}
	if (hour > 23)
	{
		++day;
		hour = 0;
	}
	if (day > 30)
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

	time += std::to_string(minute) + ':';
	time += "00  ";

	time += std::to_string(day + 1) + '/';
	time += std::to_string(month + 1) + '/';
	time += std::to_string(year) + '/';

	return time;
}
