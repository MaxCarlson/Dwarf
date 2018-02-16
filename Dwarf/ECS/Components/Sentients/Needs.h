#pragma once
#include "ECS\Component.h"
#include <cereal\types\functional.hpp>

constexpr double MAX_NEED_FULFILMENT = 1000.0;

struct Need
{
	Need() = default;
	Need(double lvl, double bonus, double declineRate)
		: lvl(lvl), bonus(bonus), declineRate(declineRate) {}

	double lvl = 500.0;
	double bonus = 500.0;
	const double declineRate = 1.0;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(lvl, bonus, declineRate);
	}
};

const Need Hunger(500.0, 400.0, 1.0);
const Need Thirst(500.0, 600.0, 1.2);
const Need Sleep(500.0,  350.0, 0.6);

enum class NeedIdx { THRIST, HUNGER, SLEEP };

struct Needs : public Component
{

	std::vector<Need> needs = { Thirst, Hunger, Sleep };

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(needs);
	}
};

CEREAL_REGISTER_TYPE(Needs);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Needs);