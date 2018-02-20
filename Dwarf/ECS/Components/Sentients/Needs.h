#pragma once
#include "ECS\Component.h"
#include <cereal\types\functional.hpp>

constexpr double MAX_NEED_FULFILMENT = 1000.0;

struct Need
{
	Need() = default;
	Need(double lvl)
		: lvl(lvl) {}

	double lvl = 750.0;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(lvl);
	}
};

const Need Hunger(750.0);
const Need Thirst(750.0);
const Need Sleep(1000.0);
const Need Comfort(750.0);
const Need Joy(750.0);

enum class NeedIdx { THRIST, HUNGER, SLEEP, COMFORT, JOY };

// Base decline rates for needs
namespace NeedDeclines
{
	constexpr double thirst = 0.026;
	constexpr double hunger = 0.026;
	constexpr double sleep = 0.026;
	constexpr double comfort = 0.026;
	constexpr double joy = 0.026;
}

// Need thresholds for thoughts
namespace ThristThreshold
{
	constexpr double THIRSTY = 400.0;
	constexpr double PARCHED = 250.0;
	constexpr double VERY_THRISTY = 0.1;
	constexpr double DEHYDRATED = 0.0;
}

namespace HungerThreshold
{
	constexpr double HUNGRY = 400.0;
	constexpr double FAMISHED = 250.0;
	constexpr double URGENTLY = 0.1;
	constexpr double STARVING = 0.0;
}

namespace SleepThreshold
{
	// Anything greater is rested
	constexpr double SLEEPY = 400.0;
	constexpr double TIRED = 250.0;
	constexpr double VERY_TIRED = 0.1;
	constexpr double EXHAUSTED = 0.0;
}

// Job priorities to fill needs // TODO: Make these values modifiable in game, perhaps even differently depending on Entity
// depending on need level
namespace ThirstPriority
{
	constexpr int QUENCHED = 0;
	constexpr int THIRSTY = 19;
	constexpr int PARCHED = 20;
	constexpr int VERY_THRISTY = 22;
	constexpr int DEHYDRATED = 32;
}

namespace HungerPriority
{
	constexpr int FULL = 0;
	constexpr int HUNGRY = 19;
	constexpr int FAMISHED = 20;
	constexpr int URGENTLY_HUNGRY = 22; 
	constexpr int STARVING = 31;
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