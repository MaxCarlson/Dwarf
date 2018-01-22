#include "stdafx.h"
#include "ReadPlants.h"
#include "Lua.h"
#include "raws.h"
#include "Defs\PlantDef.h"

boost::container::flat_map<std::string, size_t> plantDefIdx;
std::vector<PlantDef> plantDefs;

size_t getPlantIdx(const std::string & tag) noexcept
{
	auto find = plantDefIdx.find(tag);

	if (find != plantDefIdx.end())
		return find->second;

	return 0;
}

PlantDef * getPlantDef(const size_t idx) noexcept
{
	if(idx < 0 || idx > plantDefs.size())
		return nullptr;

	return &plantDefs[idx];
}

std::vector<PlantDef>* getAllPlantDefs() noexcept
{
	return &plantDefs;
}

void readInPlants() noexcept
{
	vchar v;
	PlantDef p;
	std::string tag;

	readLuaTable("vegetation",
		[&p, &tag](const auto &key) { tag = key; p = PlantDef{}; p.tag = tag; },
		[&p, &tag](const auto &key) { plantDefs.emplace_back(p); plantDefIdx[tag] = plantDefs.size() - 1; },
		luaParser
		{
			{"name", [&p]() { p.name = lua_str(); }},
			{"cycles", [&p]() {
				readLuaInnerT("cycles", [&p](auto cycle) { p.lifecycle.emplace_back(std::stoi(cycle)); });
			}},
			{"glyphs", [&p, &v]() {
				readLuaMultiTable("glyphs",
					[&p, &v]() { v = vchar{}; },
					[&p, &v](const auto str) 
						{
							if (str == "glyph") v.c  = lua_int();
							if (str == "col")   v.fg = color_from_name(lua_str().c_str());
						},
					[&p, &v]() { p.chars.emplace_back(v); }
				);
			}},
			{"harvest", [&p]() {
				readLuaInnerT("harvest", [&p](auto h) { p.harvestsTo.emplace_back(h); });
			}},
			{"tags", [&p]() {
				readLuaInnerT("tags", [&p](auto t)
				{
					if (t == "spread") p.tags.set(PLANT_SPREADS);
					if (t == "annual")  p.tags.set(PLANT_ANNUAL);
				});
			}},
			{"difficulty", [&p]() {
				readLuaInnerT("difficulty", [&p](auto d)
				{
					if (d == "diff")  p.difficulty       = lua_int();
					if (d == "ptime") p.time.first       = lua_double() * 1000;
					if (d == "htime") p.time.second      = lua_double() * 1000;
					if (d == "lvl")   p.levelRequirement = lua_int();
				});
			}},
		}
	);
}

void sanityCheckPlants() noexcept
{
}
