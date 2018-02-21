#pragma once
#include "../Component.h"

struct Stats;

enum QualityDef
{
	QUALITY_NONE,
	AWFUL_QUALITY = 5,
	POOR_QUALITY = 10,
	NORMAL_QUALITY = 15,
	GOOD_QUALITY = 27,
	SUPERIOR_QUALITY = 38,
	EPIC_QUALITY = 47,
	LEGENDAY_QUALITY = 50
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


inline QualityDef calculateQuality(Stats &stats, const std::string& skill, const int difficulty); // TODO: Balance this!

inline const std::string& getQualityName(const int quality);


CEREAL_REGISTER_TYPE(Quality);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Quality);
