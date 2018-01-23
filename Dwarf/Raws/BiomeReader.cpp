#include "stdafx.h"
#include "BiomeReader.h"
#include "Defs\BiomeDef.h"
#include "Lua.h"

std::vector<BiomeDef> biomes;

BiomeDef * getBiomeDef(const std::size_t idx)
{
	return &biomes[idx];
}

void readInBiomes() noexcept
{
	BiomeDef b;
	std::string tag;

	readLuaTable("biomes",
		[&b, &tag](auto type) {tag = type; b = BiomeDef{}; },
		[&b, &tag](auto type) { biomes.emplace_back(b); },
		luaParser
		{
			{"name", [&b]() {b.name = lua_str(); }},
			{"minTemp", [&b]() {b.minTemp = lua_int(); }},
			{"maxTemp", [&b]() {b.maxTemp = lua_int(); }},
			{ "minRain", [&b]() {b.minRain = lua_int(); } },
			{ "maxRain", [&b]() {b.maxRain = lua_int(); } },
			{"occurs", [&b]() {
				readLuaInnerT("occurs", [&b](auto t)
				{
					b.occurs.emplace_back(lua_int());
				});
			}},
			{ "soils", [&b]() {
				readLuaInnerT("soils", [&b](auto t)
				{
					if (t == "soil") b.soil = lua_int();
					if (t == "sand") b.sand = lua_int();
				});
			}},
			{ "rend", [&b]() {
				readLuaInnerT("rend", [&b](auto t)
				{
					if (t == "color") b.glyph.fg = color_from_name(lua_str().c_str());
					if (t == "glyph") b.glyph.c = lua_int();
				});
			}},
			{ "plants", [&b]() {
				readLuaInnerT("plants", [&b](auto t)
				{
					b.plants.emplace_back(std::make_pair(t, lua_int()));
				});
			}},
			{ "trees", [&b]() {
				readLuaInnerT("trees", [&b](auto t)
				{
					if (t == "evergreen") b.evergreenChance = lua_int();
					if (t == "deciduous") b.deciduosChance = lua_int();
				});
			}},
		}
	);

	int a = 5;
}

void sanityCheckBiomes() noexcept
{
}
