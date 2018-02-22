#pragma once
#include "../Component.h"

class Stats;

struct Quality : public Component
{
	Quality() = default;
	Quality(int quality) : quality(quality) {}

	int quality = QUALITY_NONE;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(quality);
	}
};


int calculateQuality(Stats &stats, const std::string& skill, const int difficulty); // TODO: Balance this!

const std::string& getQualityName(const Entity &e);


CEREAL_REGISTER_TYPE(Quality);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Quality);
