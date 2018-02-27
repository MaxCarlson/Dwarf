#include "stdafx.h"
#include "ReadBodyParts.h"
#include "Defs\BodyPart.h"
#include "Lua.h"

std::unordered_map<std::string, BodyPartDef> bodyParts;
std::unordered_map<std::string, BodyDef> bodyDefs;

BodyPartDef * getBodyPart(const std::string & tag)
{
	auto find = bodyParts.find(tag);

	if (find != bodyParts.end())
		return &find->second;

	return nullptr;
}

BodyDef * getBodyDef(const std::string & tag) noexcept
{
	auto find = bodyDefs.find(tag);

	if (find != bodyDefs.end())
		return &find->second;

	return nullptr;
}

void readInBodyParts() noexcept
{
	BodyPartDef b;
	std::string tag;

	readLuaTable("body_parts", 
		[&](auto s) { tag = s; },
		[&](auto s) { bodyParts[tag] = b; b.tag = tag; },
		luaParser {
			{ "name", [&]() { b.name = lua_str(); } },
			{ "description", [&]() { b.description = lua_str(); } },
			{ "health", [&]() { b.health = lua_int(); } },
		}		
	);
}

void readyInBodyDefs() noexcept
{
	BodyDef b;
	std::string tag;

	readLuaTable("body_defs",
		[&](auto s) { tag = s; },
		[&](auto s) { bodyDefs[tag] = b; b.tag = tag; },
		luaParser {


		}
	);
}

void sanityCheckBodyParts() noexcept
{

}

void sanityCheckBodyDefs() noexcept
{
}
