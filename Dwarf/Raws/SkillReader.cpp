#include "stdafx.h"
#include "SkillReader.h"
#include "Lua.h"
#include <boost\container\flat_map.hpp>

// Returns index of attribute
boost::container::flat_map<std::string, int> attributes;

// Indexed by attribute tag
// vector of skills associated with attribute
boost::container::flat_map<std::string, std::vector<std::string>> skills;

void readInSkills() noexcept
{
	// Read in attribute indicies
	std::string name;
	int id = 0;

	readLuaTable("attributes",
		[](const auto &key) {},
		[&id, &name](const auto &key) { attributes[name] = id; },
		luaParser
		{
			{ "name", [&name]() { name = lua_str(); } },
			{ "id",[&id]() { id = lua_int(); }  }
		}
	);

	// Read in skills
	std::string tag;
	readLuaTable("skills",
		[&tag](const auto &key) {},
		[](const auto &key) {},
		luaParser
		{
			{ "name", [&tag]() { tag = lua_str(); } },
			{ "s_attributes", [&tag]() {
				readLuaMultiTable("s_attributes",
					[]() {},
					[&tag](auto type) {
						if (type == "attr") skills[tag].push_back(lua_str());

					},
					[]() {}
				);
				}
			}
		}
	);

	int a = 5;
}

void sanityCheckSkills() noexcept
{
}
