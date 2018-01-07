#pragma once
#include "../../Component.h"
#include "../Helpers/skill.h"
#include <unordered_map>

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

void doWorkTime(const double & tstep, double &progress, const int difficulty);

CEREAL_REGISTER_TYPE(Stats);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Stats);