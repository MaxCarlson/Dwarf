#include "stdafx.h"
#include "Lua.h"
#include "ReadSpecies.h"
#include "Defs\SpeciesDef.h"
#include "boost\container\flat_map.hpp"

boost::container::flat_map<std::string, SpeciesDef> speciesMap;


SpeciesDef * getSpecies(const std::string & tag) noexcept
{
	return nullptr;
}

void readInSpecies() noexcept
{
	SpeciesDef d;

	readLuaTable("species",
		[&](auto s) { d = SpeciesDef{}; d.tag = s; },
		[&](auto s) { speciesMap[s] = d; },
		luaParser{
			{ "name",			[&]() { d.name			= lua_str(); } },
			{ "description",	[&]() { d.description	= lua_str(); } },
			{ "body",			[&]() { d.bodyTag		= lua_str(); } },
			
		}
	);
}

void sanityCheckSpecies() noexcept
{
}
