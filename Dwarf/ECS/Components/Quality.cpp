#include "stdafx.h"
#include "Quality.h"
#include "Sentients\Stats.h"
#include "helpers\Rng.h"

static const std::vector<std::string> qualityNames = { "None", "Awful", "Poor", "Normal", "Good", "Superior", "Epic", "Legendary" };


QualityDef calculateQuality(Stats &stats, const std::string &skill, const int difficulty) // TODO: Better method
{
	int stat = 1;
	auto sfind = stats.skills.find(skill);

	if (sfind != stats.skills.end())
		stat = sfind->second.skillLvl + 1;

	auto roll = rng.range(1, stat * 4);

	roll -= difficulty;

	const auto luck = rng.range(1, stat / 2);

	roll += luck;

	if (roll <= AWFUL_QUALITY)
		return AWFUL_QUALITY;
	else if (roll <= POOR_QUALITY)
		return POOR_QUALITY;
	else if (roll <= NORMAL_QUALITY)
		return NORMAL_QUALITY;
	else if (roll <= GOOD_QUALITY)
		return GOOD_QUALITY;
	else if (roll <= SUPERIOR_QUALITY)
		return SUPERIOR_QUALITY;
	else if (roll <= EPIC_QUALITY)
		return EPIC_QUALITY;
	else
		return LEGENDAY_QUALITY;
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

