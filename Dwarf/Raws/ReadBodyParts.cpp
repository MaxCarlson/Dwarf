#include "stdafx.h"
#include "ReadBodyParts.h"
#include "Defs\BodyPart.h"
#include "Lua.h"

std::unordered_map<std::string, BodyPartDef> bodyParts;

BodyPartDef * getBodyPart(const std::string & tag)
{
	auto find = bodyParts.find(tag);

	if (find != bodyParts.end())
		return &find->second;

	return nullptr;
}

void readInBodyParts() noexcept
{
	BodyPartDef b;
	std::string tag;

	readLuaTable("body_parts", 
		[&](auto s) { tag = s; },
		[&](auto s) { bodyParts[tag] = b; },
		luaParser {
			{ "name", [&]() { b.name = lua_str(); } },
			{ "description", [&]() { b.description = lua_str(); } },
			{ "health", [&]() { b.health = lua_int(); } },
		}		
	);
}

void sanityCheckBodyParts() noexcept
{

}
