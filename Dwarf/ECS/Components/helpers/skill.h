#pragma once
#include <cereal\cereal.hpp>

struct attribute
{
	attribute() = default;
	attribute(int attributeLvl, int experience) : attributeLvl(attributeLvl), experience(experience) {}

	int attributeLvl = 0;
	int experience = 0;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(attributeLvl, experience);
	}
};

struct skill
{
	skill() = default;
	skill(int skillLvl, int experience) : skillLvl(skillLvl), experience(experience) {}

	int skillLvl = 0;
	int experience = 0;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(skillLvl, experience);
	}
};