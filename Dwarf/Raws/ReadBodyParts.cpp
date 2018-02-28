#include "stdafx.h"
#include "ReadBodyParts.h"
#include "Defs\BodyPart.h"
#include "Lua.h"

std::unordered_map<std::string, BodyPartDef> bodyParts;
std::unordered_map<std::string, BodyDef> bodyDefs;

BodyPartDef * getBodyPart(const std::string & tag) noexcept
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

	readLuaTable("body_parts", 
		[&](auto s) { b = BodyPartDef {}; b.tag = s; },
		[&](auto s) { bodyParts[b.tag] = b; },
		luaParser {
			{ "name", [&]() { b.name = lua_str(); } },
			{ "description", [&]() { b.description = lua_str(); } },
			{ "health", [&]() { b.health = lua_int(); } },
		}		
	);
}

inline void recurse(const std::string field)
{
	lua_pushstring(luaState, field.c_str());
	lua_gettable(luaState, -2);
}

void test(const std::string table)
{

}


void readyInBodyDefs() noexcept
{
	BodyDef b;

	lua_getglobal(luaState, "body_defs");
	lua_pushnil(luaState);

	std::vector<SpeciesBodyPart> spParts;

	while (lua_next(luaState, -2) != 0)
	{
		const std::string key = lua_tostring(luaState, -2);

		lua_pushstring(luaState, key.c_str());
		lua_gettable(luaState, -2);

		while (lua_next(luaState, -2) != 0)
		{
			const std::string field = lua_tostring(luaState, -2);

			lua_pushstring(luaState, field.c_str());
			lua_gettable(luaState, -2);

			SpeciesBodyPart newPart;
			auto * part = getBodyPart(field);

			newPart.tag = part->tag;

			while (lua_next(luaState, -2) != 0)
			{
				const std::string nf = lua_tostring(luaState, -2);

				if (nf == "qty")
				{

				}
				if (nf == "att")
				{
					newPart.attachedTo = lua_str();
				}

				lua_pop(luaState, 1);
			}

			lua_pop(luaState, 1);
		}

		lua_pop(luaState, 1);
	}

	int a = 5;
}

void sanityCheckBodyParts() noexcept
{

}

void sanityCheckBodyDefs() noexcept
{
}
/*

human_body = { -- TODO: Possibly make this a func accepting a multiplier for health?
head = {
{skull = 1},
{brain = 1},
{eye = 2},
{ear = 2},
{nose = 1},
{neck = 1}
},
toroso = {
{arm = 2, parts = {
{hand = 1, parts = {
{ finger = 5 },
}},
}},
{heart = 1},
{lung = 2},
{stomach = 1},
{liver = 1},
{kidney = 2}
},
waist = {
{leg = 2},
{foot = 2},
{toe = 10}
}
},

*/