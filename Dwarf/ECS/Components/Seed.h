#pragma once
#include "ECS\Component.h"

struct Coordinates;

struct Seed : public Component
{
	Seed() = default;
	Seed(const std::string& plantTag) : plantTag(plantTag) {}

	std::string plantTag;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(plantTag);
	}
};
CEREAL_REGISTER_TYPE(Seed);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Seed);

void spawnSeeds(const int num, const Coordinates& co, const std::string &plantTag);