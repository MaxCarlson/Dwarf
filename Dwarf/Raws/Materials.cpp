#include "Materials.h"
#include "Defs\MaterialDef.h"
#include "boost\container\flat_map.hpp"
#include "Lua.h"

boost::container::flat_map<std::string, std::size_t> materialDefsIdx;
std::vector<MaterialDef> materialDefs;

MaterialDef * returnMaterial(const std::string & tag)
{
	auto idx = materialDefsIdx.at(tag);

	return &materialDefs[idx];
}

MaterialDef * returnMaterial(const int idx)
{
	return &materialDefs[idx];
}

void readInMaterials()
{
	std::string tag;
	MaterialDef m;

	readLuaTable("materials",
		[&m, &tag](const auto &key) { tag = key; m = MaterialDef{}; m.tag = tag; },
		[&m, &tag](const auto &key) { materialDefs[tag] = m; },
			luaParser{
				{ "name", [&m]() { m.name = lua_str(); } },
				{ "description", [&m]() { m.description = lua_str(); } },
				{ "color", [&m]() { m.color = lua_str(); } },
				{ "glyph", [&m]() { m.charCode = lua_int(); } },
				{ "tcode", [&m]() { m.tilesetKey = lua_int(); } },
				{ "health", [&m]() { m.health = lua_int(); } },
				{ "minesTo", [&m]() { m.minesToTag = lua_str(); } },
				{ "mineAmt", [&m]() { m.minesToAmount = lua_int(); } },
				{"matType", [&m]() {
				readLuaInnerT("matType", [&m](auto type) {
					if (type == "rock")  m.matType == MAT_ROCK;
					if (type == "soil")  m.matType == MAT_SOIL;
					if (type == "sand")  m.matType == MAT_SAND;
					if (type == "metal") m.matType == MAT_METAL;
				});
			}}
		}
	);
}

void sanityCheckMaterials()
{
}
