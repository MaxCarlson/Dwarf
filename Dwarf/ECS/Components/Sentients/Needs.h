#pragma once
#include "ECS\Component.h"
#include <cereal\types\functional.hpp>

constexpr double MAX_NEED_FULFILMENT = 1000.0;

struct Need
{
	Need() = default;
	Need(double lvl, double declineRate)
		: lvl(lvl), declineRate(declineRate) {}

	double lvl = 500.0;
	const double declineRate = 1.0;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(lvl, bonus, declineRate);
	}
};

const Need Hunger(500.0, 1.0);
const Need Thirst(500.0, 1.2);
const Need Sleep(500.0,  0.6);
const Need Comfort(750.0, 0.45);
const Need Joy(750.0, 0.45);

enum class NeedIdx { THRIST, HUNGER, SLEEP, COMFORT, JOY };

namespace SleepThreshold
{
	constexpr double SLEEPY = 400.0;
	constexpr double TIRED = 250.0;
	constexpr double VERY_TIRED = 0.1;
	constexpr double EXHAUSTED = 0.0;
}

namespace SleepPriority
{
	constexpr int RESTED = 0;
	constexpr int SLEEPY = 20;
	constexpr int TIRED = 21;
	constexpr int VERY_TIRED = 22;
	constexpr int EXHAUSTED = 30;
}

struct Needs : public Component
{

	std::vector<Need> needs = { Thirst, Hunger, Sleep, Comfort, Joy };

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(needs);
	}
};

CEREAL_REGISTER_TYPE(Needs);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Needs);