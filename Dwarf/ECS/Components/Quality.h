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

inline double qualityMeleeMultiplier(int quality)
{
	switch (quality)
	{
	case QUALITY_NONE:
		return 1.0;
	case AWFUL_QUALITY:
		return 0.4;
	case POOR_QUALITY:
		return 0.7;
	case NORMAL_QUALITY:
		return 0.95;
	case GOOD_QUALITY:
		return 1.1;
	case SUPERIOR_QUALITY:
		return 1.2;
	case EPIC_QUALITY:
		return 1.35;
	case LEGENDAY_QUALITY:
		return 1.55;
	default:
		return 1.0;
	}
}


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

const std::string& getQualityName(const int quality);
const std::string& getQualityName(const Entity &e);


CEREAL_REGISTER_TYPE(Quality);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Quality);
