#include "../stdafx.h"
#include "Buildings.h"
#include "Defs\BuildingDef.h"
#include "Materials.h"
#include "Lua.h"

boost::container::flat_map<std::string, BuildingDef> buildingDefs;

BuildingDef * getBuilding(const std::string & tag)
{
	auto find = buildingDefs.find(tag);
	if (find != buildingDefs.end())
		return &find->second;

	return nullptr;
}

const std::vector<std::string> get_all_building_def_names()
{
	std::vector<std::string> names;
	for (const auto& b : buildingDefs)
	{
		names.push_back(b.second.name);
	}
	return names;
}

const std::vector<std::string> get_building_tags()
{
	std::vector<std::string> tags;
	for (const auto& b : buildingDefs)
	{
		tags.push_back(b.first);
	}
	return tags;
}

void vecCharIdx(BuildingDef & b, int idx, uint16_t c)
{
	if (b.charCodes.size() < idx)
		b.charCodes.resize(idx);

	b.charCodes[idx - 1] = c;
}

void readInBuildings() noexcept
{
	BuildingDef b;
	std::string tag;
	ReactionInput inp;

	//std::vector<BuildingDef> tester; // For easy viewing of buildings

	readLuaTable("buildings", 
		[&b, &tag](const auto& key) { tag = key; b = BuildingDef{}; b.tag = tag; },
		[&b, &tag](const auto& key) { buildingDefs[tag] = b; }, // tester.push_back(b); },
		luaParser
		{
			{ "name", [&b]() {b.name = lua_str(); }},
			{ "description", [&b]() { b.description = lua_str(); }},
			{ "width", [&b]() { b.width = lua_int(); }},
			{ "height", [&b]() { b.height = lua_int(); } },
			{ "structure", [&b]() { b.structure = true; } },
			{ "components", [&b, &inp]() {
			readLuaMultiTable("components",
					[&inp]() { inp = ReactionInput{}; },
					[&b, &inp](auto type2) {
						if (type2 == "item") inp.tag = lua_str();
						if (type2 == "qty") inp.quantity = lua_int();
					},
					[&b, &inp]() { 	b.components.push_back(inp); }
				);
			}},
			{"provides", [&b]() {
				readLuaTable2D("provides",
					[  ](auto type1) {},
					[&b](auto type2) {
						if (type2 == "workshop")	 b.provides.set(PROVIDES_WORKSHOP);
						if (type2 == "wall")		 b.provides.set(PROVIDES_WALL);
						if (type2 == "floor")		 b.provides.set(PROVIDES_FLOOR);
						if (type2 == "ramp")		 b.provides.set(PROVIDES_RAMP);
						if (type2 == "upStaits")	 b.provides.set(PROVIDES_UP_STAIRS);
						if (type2 == "downStairs")   b.provides.set(PROVIDES_DOWN_STAIRS);
						if (type2 == "upDownStairs") b.provides.set(PROVIDES_UP_DOWN_STAIRS);
						if (type2 == "sleep")        b.provides.set(PROVIDES_SLEEP);
					} 
				);
				}		
			},	
			{"skill", [&b]() { 
				readLuaTable2D("skill",
					[&b](auto type1) {},
					[&b](auto type2) {
						if (type2 == "name") b.skill_required.first = lua_str();
						if (type2 == "difficulty") b.skill_required.second = lua_int();
					}
				);
				}
			},
			{ "render", [&b]() { 
				readLuaMultiTable("render",
					[  ]() {},
					[&b](auto type2) {
						if (type2 == "glyph") b.charCodes.push_back(lua_int()); // Is this always in correct order? // Looks like it!?
					},
					[]() {}
				);
				}
			},
		}
	);
}

void sanityCheckBuildings() noexcept
{
}
