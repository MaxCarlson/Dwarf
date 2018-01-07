#include "stdafx.h"
#include "SkillReader.h"
#include "Lua.h"
#include <boost\container\flat_map.hpp>
#include <boost\container\flat_set.hpp>

// All attributes in existance
std::vector<std::string> attributes;
std::vector<std::string> skills;

// Indexed by skill tag
// vector of attributes, skills[0].second[0] is the skill that effects speed, success chance, etc
// everything after just get's a minor boost in xp when performing skill
boost::container::flat_map<std::string, std::vector<std::string>> skillAttributes;

// Visually pleasing skill names indexed by skill tag
// Entire purpose is to remove underscores. Is this needed?
boost::container::flat_map<std::string, std::string> skillNames;


const std::vector<std::string>& allAttributes()
{
	return attributes;
}

const std::vector<std::string>& allSkills()
{
	return skills;
}

const std::vector<std::string>* attributesBySkill(const std::string & skill)
{
	auto find = skillAttributes.find(skill);

	if (find != skillAttributes.end())
		return &find->second;

	return nullptr;
}

void readInSkills() noexcept
{
	// Read in attribute indicies
	std::string name;
	int id = 0;

	readLuaTable("attributes",
		[](const auto &key) {},
		[&id, &name](const auto &key) { attributes.emplace_back(name); },
		luaParser
		{
			{ "name", [&name]() { name = lua_str(); } },
			//{ "id",[&id]() { id = lua_int(); }  }
		}
	);

	std::vector<std::vector<std::string>> test;
	std::vector<std::string> keys;

	// Read in skills
	std::string tag;
	readLuaTable("skills",
		[&tag](const auto &key) { tag = key; },
		[](const auto &key) {},
		luaParser
		{
			{ "name", [&tag]() { skillNames[tag] = lua_str(); } },

			{ "s_attributes", [&tag, &test, &keys]() {
				readLuaMultiTable("s_attributes",
					[]() {},
					[&tag](auto type) {
						if (type == "attr") skillAttributes[tag].emplace_back(lua_str());

					},
					[]() {}
				);
				keys.push_back(tag);
				test.push_back(skillAttributes[tag]);
				}
			}
		}
	);
	int a = 5;
}

void sanityCheckSkills() noexcept
{
}
