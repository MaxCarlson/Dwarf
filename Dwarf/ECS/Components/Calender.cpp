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
