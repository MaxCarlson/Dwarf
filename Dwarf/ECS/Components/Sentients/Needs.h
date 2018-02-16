#pragma once
#include "ECS\Component.h"
#include <cereal\types\functional.hpp>

constexpr double MAX_NEED_FULFILMENT = 1000.0;

struct Need
{
	Need() = default;
	Need(double lvl, double declineModifier, double declineRate)
		: lvl(lvl), declineModifier(declineModifier), declineRate(declineRate) {}

	double lvl = 500.0;
	double declineModifier = 1.0;
	const double declineRate = 1.0;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(lvl, declineModifier, declineRate);
	}
};

struct Needs : public Component
{

	std::unordered_map<std::string, Need> needs;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(needs);
	}
};

CEREAL_REGISTER_TYPE(Needs);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Needs);