#include "Materials.h"
#include "Defs\MaterialDef.h"
#include "boost\container\flat_map.hpp"
#include "Lua.h"

#include <algorithm>

boost::container::flat_map<std::string, std::size_t> materialDefsIdx;
std::vector<MaterialDef> materialDefs;

MaterialDef * getMaterial(const std::string & tag)
{
	auto it = materialDefsIdx.find(tag);

	if (it != materialDefsIdx.end())
	{
		return &materialDefs[it->second];
	}

	return nullptr;
}

MaterialDef * getMaterial(const std::size_t idx)
{
	if(idx < materialDefs.size())
		return &materialDefs[idx];

	return nullptr;
}

const std::size_t getMaterialIdx(const std::string & tag)
{
	auto find = materialDefsIdx.find(tag);
	if(find != materialDefsIdx.end())
		return find->second;

	return 0;
}


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
				{ "layer", [&m]() {m.layer = lua_str(); } },
				{ "description", [&m]() { m.description = lua_str(); } },
				{ "color", [&m]() { m.color = lua_str(); } },
				{ "glyph", [&m]() { m.charCode = static_cast<uint16_t>(lua_tonumber(luaState, -1)); } },
				{ "floorGlyph", [&m]() {m.floorCode = static_cast<uint16_t>(lua_tonumber(luaState, -1)); } },
				{ "tcode", [&m]() { m.tilesetKey = lua_int(); } },
				{ "health", [&m]() { m.health = lua_int(); } },
				{ "minesTo", [&m]() { m.minesToTag = lua_str(); } },
				{ "mineAmt", [&m]() { m.minesToAmount = lua_int(); } },
				{ "ores", [&m]() { m.oreProduct = lua_str(); } }, // Add functionality for multiple ore types?
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

	// Set index 0 to no material
	materialDefs.insert(materialDefs.begin(), MaterialDef{});

	for (auto matIdx = 1; matIdx < materialDefs.size(); ++matIdx)
		materialDefsIdx[materialDefs[matIdx].tag] = matIdx;
}

void sanityCheckMaterials()
{
	
}

void getStrataLayers(std::vector<std::size_t> &soils, std::vector<std::size_t> &sands, std::vector<std::size_t> &igneous, std::vector<std::size_t> &sedimentries, std::vector<std::size_t> &metamorphics)
{
	std::size_t i = 0;
	for (auto it : materialDefs)
	{
		if (it.layer == "all_rock")
		{
			sedimentries.push_back(i);
			igneous.push_back(i);
			metamorphics.push_back(i);
		}
		else
		{
			if (it.matType == MAT_SOIL) soils.push_back(i);
			if (it.matType == MAT_SAND) sands.push_back(i);
			if (it.matType == MAT_ROCK && it.layer == "sedimentary") sedimentries.push_back(i);
			if (it.matType == MAT_ROCK && it.layer == "igneous") igneous.push_back(i);
			if (it.matType == MAT_ROCK && it.layer == "metamorphic") metamorphics.push_back(i);
		}
		++i;
	}
}
