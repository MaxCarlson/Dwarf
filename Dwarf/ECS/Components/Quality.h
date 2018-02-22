#pragma once
#include "../Component.h"

class Stats;

enum QualityDef
{
	QUALITY_NONE = -1,
	AWFUL_QUALITY,
	POOR_QUALITY,
	NORMAL_QUALITY,
	GOOD_QUALITY,
	SUPERIOR_QUALITY,
	EPIC_QUALITY,
	LEGENDAY_QUALITY
};


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
