#include "Materials.h"
#include "Defs\MaterialDef.h"
#include "boost\container\flat_map.hpp"
#include "Lua.h"

boost::container::flat_map<std::string, std::size_t> materialDefsIdx;
std::vector<MaterialDef> materialDefs;

MaterialDef * getMaterial(const std::string & tag)
{
	auto idx = materialDefsIdx.at(tag);

	return &materialDefs[idx];
}

MaterialDef * getMaterial(const int idx)
{
	return &materialDefs[idx];
}

const std::size_t getMaterialIdx(const std::string & tag)
{
	return materialDefsIdx[tag];
}
#include <algorithm>

void readInMaterials() noexcept
{
	std::string tag;
	MaterialDef m;
	int matIdx = 0;

	readLuaTable("materials",
		[&m, &tag](const auto &key) { tag = key; m = MaterialDef{}; m.tag = tag; },
		[&m, &tag, &matIdx](const auto &key) { materialDefsIdx[tag] = matIdx++; materialDefs.push_back(m); },
			luaParser{
				{ "name", [&m]() { m.name = lua_str(); } },
				{ "description", [&m]() { m.description = lua_str(); } },
				{ "color", [&m]() { m.color = lua_str(); } },
				{ "glyph", [&m]() { m.charCode = lua_int(); } },
				{ "tcode", [&m]() { m.tilesetKey = lua_int(); } },
				{ "health", [&m]() { m.health = lua_int(); } },
				{ "minesTo", [&m]() { m.minesToTag = lua_str(); } },
				{ "mineAmt", [&m]() { m.minesToAmount = lua_int(); } },
				{ "matType", [&m]() {
				readLuaInnerT("matType", [&m](auto type) {
					if (type == "rock")		 m.matType = MAT_ROCK;
					if (type == "soil")		 m.matType = MAT_SOIL;
					if (type == "sand")		 m.matType = MAT_SAND;
					if (type == "metal")     m.matType = MAT_METAL;
					if (type == "synthetic") m.matType = MAT_SYTHETIC;
					if (type == "organic")   m.matType = MAT_ORGANIC;
				});
			}}
		}
	);

	// Ensure order is identical each game load
	std::sort(materialDefs.begin(), materialDefs.end(), [](MaterialDef a, MaterialDef b) {return a.tag < b.tag; });

	for (auto matIdx = 0; matIdx < materialDefs.size(); ++matIdx)
		materialDefsIdx[materialDefs[matIdx].tag] = matIdx;
}

void sanityCheckMaterials()
{
}
