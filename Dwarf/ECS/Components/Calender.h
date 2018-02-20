#pragma once
#include "../Component.h"

class Calender : public Component
{
public:

	double minute = 0.0;
	int hour = 0;
	int day = 0;
	int month = 0;
	int year = 0;

	void nextMinute();

	std::string getTime();

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(minute, hour, day, month, year);
	}
};

CEREAL_REGISTER_TYPE(Calender);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Calender);