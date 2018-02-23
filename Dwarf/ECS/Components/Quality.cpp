#include "stdafx.h"
#include "Quality.h"
#include "Sentients\Stats.h"
#include "helpers\Rng.h"

#include <random>

static const std::vector<std::string> qualityNames = { "None", "Awful", "Poor", "Normal", "Good", "Superior", "Epic", "Legendary" };

std::default_random_engine gen;

int calculateQuality(Stats &stats, const std::string &skill, const int difficulty) // TODO: Factor in attributes ? // TODO: Use rng class!
{
	int stat = 0;
	auto sfind = stats.skills.find(skill);

	if (sfind != stats.skills.end())
		stat = sfind->second.skillLvl;

	// Average of one quality level increase per 3 skill levels
	double mean = static_cast<double>(stat) / 3.0;
	std::normal_distribution<double> distri(mean, 1.25);

	int roll = static_cast<int>(distri(gen));

	if (roll < AWFUL_QUALITY)
		roll = AWFUL_QUALITY;
	else if (roll > LEGENDAY_QUALITY)
		roll = LEGENDAY_QUALITY;
	
	return roll;
}

const std::string & getQualityName(const int quality)
{
	switch (quality)
	{
	case QUALITY_NONE:
		return qualityNames[0];
	case AWFUL_QUALITY:
		return qualityNames[1];
	case POOR_QUALITY:
		return qualityNames[2];
	case NORMAL_QUALITY:
		return qualityNames[3];
	case GOOD_QUALITY:
		return qualityNames[4];
	case SUPERIOR_QUALITY:
		return qualityNames[5];
	case EPIC_QUALITY:
		return qualityNames[6];
	case LEGENDAY_QUALITY:
		return qualityNames[7];
	default:
		return qualityNames[0];
	}
}

const std::string & getQualityName(const Entity &e)
{
	if (!e.hasComponent<Quality>())
		return qualityNames[0];

	const int quality = e.getComponent<Quality>().quality;

	return getQualityName(quality);
}

