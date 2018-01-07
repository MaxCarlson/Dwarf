#include "stdafx.h"
#include "Stats.h"
#include "../../../Raws/SkillReader.h"

#include <random> // Once we have a planet seeded rng with determinstic behavior this will be cut
static std::random_device rd;
static std::mt19937 gen(rd());
template<int lower, int upper>
int randBound()
{
	static std::uniform_int_distribution<> dist(lower, upper);

	return dist(gen);
}

inline int attributeModifier(int lvl)
{
	if (lvl == 1)
		return -5;
	return ((lvl - 1) / 2) - 4;
}

int getMainAttribute(Stats & stats, const std::string& skilltag)
{
	// Get main attribute associated with skill
	auto* attr = attributesBySkill(skilltag);

	if (attr != nullptr && !attr->empty())
	{
		// Get index of main attribute in skills vector
		auto attribute = stats.attributes.find(attr->front());

		if (attribute != stats.attributes.end())
			return attribute->second.attributeLvl;
	}

	return 0;
}

int getSkillLvl(Stats & stats, const std::string& skilltag)
{
	auto find = stats.skills.find(skilltag);

	if (find != stats.skills.end())
	{
		return find->second.skillLvl;
	}

	return 0;
}

void gainSkill(Stats & stats, const std::string skilltag, int difficulty)
{
	auto find = stats.skills.find(skilltag);

	if (find != stats.skills.end())
	{
		// Dwarf gains experience
		find->second.experience += difficulty;

		const int gainLvlAt = (find->second.skillLvl + 1) * 150;

		// Dwarf gains a level in skill
		if (find->second.experience > gainLvlAt)
		{
			find->second.experience = 0;
			++find->second.skillLvl;
		}


		// handle attribute experience and lvl gain
		const auto attributes = attributesBySkill(skilltag);

		if (attributes != nullptr && !attributes->empty())
		{
			int count = 1;
			for (const auto at : *attributes)
			{
				auto findAttribute = stats.attributes.find(at);

				if (findAttribute != stats.attributes.end())
				{
					// Add more expience to the "More important" attributes
					// and less to the more peripheral ones
					findAttribute->second.experience += difficulty / count;

					const int lvlUpAt = (findAttribute->second.attributeLvl + 1) * 200;

					// Attribute is above experinece threshold to lvl up
					if (findAttribute->second.experience > lvlUpAt)
					{
						findAttribute->second.experience = 0;
						++findAttribute->second.attributeLvl;
					}
				}

				++count;
			}
		}
	}
	else
		stats.skills[skilltag] = skill{ 1, difficulty };
}


skill_roll skillRoll(Stats & stats, const std::string skilltag, int difficulty)
{
	const int luck = randBound<1, 25>();  // Once we have a planet seeded rng with determinstic behavior this template will be cut


	const int attribute = getMainAttribute(stats, skilltag);
	const int skilllvl = getSkillLvl(stats, skilltag);

	const int total = luck + attribute + skilllvl;

	if (luck == 1)
		return CRITICAL_FAIL;

	else if (luck == 25)
	{
		gainSkill(stats, skilltag, difficulty);
		return EPIC_SUCCESS;
	}

	else if (total >= difficulty)
	{
		gainSkill(stats, skilltag, difficulty);
		return SUCCESS;
	}
		
	return FAIL;
}
