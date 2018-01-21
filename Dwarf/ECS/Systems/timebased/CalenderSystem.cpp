#include "stdafx.h"
#include "CalenderSystem.h"
#include "Globals\global_calender.h"


void CalenderSystem::update(const double duration)
{
	auto hour = calender->hour;
	auto day  = calender->day;

	calender->nextMinute();

	hourPassed = false;
	dayPassed  = false;

	if (hour != calender->hour) hourPassed = true;
	if (day != calender->day)   dayPassed  = true; 
}
