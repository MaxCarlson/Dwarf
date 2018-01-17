#pragma once
#include "../../Component.h"
#include "../Helpers/skill.h"
#include <unordered_map>

inline int attributeModifier(int lvl) // Implement this into skills time duration calculations
{
	if (lvl == 1)
		return -5;
	return ((lvl - 1) / 2) - 4;
}

class Stats : public Component
{
public:
	Stats() = default;
	Stats(const std::unordered_map<std::string, attribute>& attributes, const std::unordered_map<std::string, skill>& skills) : attributes(attributes), skills(skills) {}

	std::unordered_map<std::string, attribute> attributes;
	std::unordered_map<std::string, skill> skills;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(attributes, skills);
	}
};

inline int getSkillLvl(Stats & stats, const std::string& skilltag)
{
	auto find = stats.skills.find(skilltag);

	if (find != stats.skills.end())
		return find->second.skillLvl;
	
	return 0;
}

enum skill_roll
{
	CRITICAL_FAIL,
	FAIL,
	SUCCESS,
	EPIC_SUCCESS
};

constexpr int DIFFICULTY_TRIVIAL   =  0;
constexpr int DIFFICULTY_SIMPLE    =  5;
constexpr int DIFFICULTY_NORMAL    = 10;
constexpr int DIFFICULTY_MODERATE  = 15;
constexpr int DIFFICULTY_TOUGH	   = 20;
constexpr int DIFFICULTY_HARD      = 25;
constexpr int DIFFICULTY_GRUELING  = 30;
constexpr int DIFFICULTY_LEGENDARY = 40;

skill_roll skillRoll(Stats & stats, const std::string skilltag, int difficulty);

void doWorkTime(Stats &stats, const std::string & skill, const double &tstep, double &progress);
int  doWorkDamage(Stats &stats, const std::string & skill);

void giveWorkXp(Stats &stats, const std::string & skill, int difficulty);

CEREAL_REGISTER_TYPE(Stats);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Stats);