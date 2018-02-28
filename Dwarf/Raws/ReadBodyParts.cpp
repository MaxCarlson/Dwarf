#include "stdafx.h"
#include "ReadBodyParts.h"
#include "Defs\BodyPart.h"
#include "Lua.h"

std::unordered_map<std::string, BodyPartDef> bodyParts;
std::unordered_map<std::string, BodyDef> bodyDefs;
std::unordered_map<std::string, std::vector<BodyPart>> bodies;

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

std::vector<BodyPart> getCreatureBody(const std::string & tag)
{
	auto find = bodies.find(tag);
	if (find != bodies.end())
		return find->second;

	throw std::runtime_error("Invalid body tag used");
	return std::vector<BodyPart>{};
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
			{ "health", [&]() { b.health = lua_double(); } },
			{ "effects", [&]() {
				readLuaInnerT("effects", [&](std::string s)
				{
					if (s == "kill")		    b.effects.set(BodyPartEffects::ON_ALL_GONE_KILL);
					if (s == "damage_mobility") b.effects.set(BodyPartEffects::DAMAGE_MOBILITY);
				});
			}},
		}		
	);
}

inline void attachPartsToBases(BodyDef &b, const std::string &base, const std::string &part) 
{
	std::vector<int> baseLocations;
	std::vector<int> partLocations;

	int i = 0;
	int baseCount = 0;
	int partCount = 0;
	for (const auto& p : b.parts)
	{
		if (p.tag == base)
		{
			baseLocations.emplace_back(i);
			++baseCount;
		}
		else if (p.tag == part)
		{
			partLocations.emplace_back(i);
			++partCount;
		}
		++i;
	}

	int partsPerBase = partCount / baseCount;

	for (auto bl : baseLocations)
	{
		// Set base part id's for partsPerBase # of 
		// body parts
		for (int i = 0; i < partsPerBase; ++i) // TODO: Make sure to fill non-even number parts bases as well
		{
			SpeciesBodyPart& sp = b.parts[partLocations.back()]; 
			partLocations.pop_back();
			sp.idAttachedTo = bl;
		}
	}
}

void buildBody(BodyDef &b) // TODO: Add in naming for left and right if there are two of each part
{
	// Sort body parts into some order,
	// doesn't really matter what it is so long as it stays the same
	std::sort(b.parts.begin(), b.parts.end(), [](SpeciesBodyPart &p, SpeciesBodyPart &p1)
	{
		return p.tag < p1.tag;
	});

	int i = 0;
	double hitIdx = 0.0;
	std::set<std::string> partDone;
	for (auto& p : b.parts)
	{
		p.id = i;
		hitIdx += p.size;
		b.hitChances.emplace_back(hitIdx);

		// If the part attaches to another part, and we haven't handled
		// it yet then attach all the parts of p.tag to all the bases close to equally
		if (p.attachedTo != "" && partDone.find(p.tag) == partDone.end())
		{
			attachPartsToBases(b, p.attachedTo, p.tag);
			partDone.insert(p.tag);
		}
		++i;
	}

	// Build the vector of body parts that will become
	// the template entities of this body type have
	auto find = bodies.find(b.tag);
	if (find == bodies.end())
	{
		bodies[b.tag] = {};
		for (const auto& p : b.parts)
		{
			bodies[b.tag].emplace_back(BodyPart{ p.id, p.tag, getBodyPart(p.tag)->health }); // TODO:  Define health not in BodyPartDef but in SpeciesBodyPart
		}
	}
	else
		throw std::runtime_error("Multiple body types with same tag!");

	b.maxChance = hitIdx;
	bodyDefs[b.tag] = b;
}

void readyInBodyDefs() noexcept
{
	lua_getglobal(luaState, "body_defs");
	lua_pushnil(luaState);

	while (lua_next(luaState, -2) != 0)
	{
		const std::string key = lua_tostring(luaState, -2);

		lua_pushstring(luaState, key.c_str());
		lua_gettable(luaState, -2);

		BodyDef body;
		body.tag = key;

		while (lua_next(luaState, -2) != 0)
		{
			const std::string field = lua_tostring(luaState, -2);

			lua_pushstring(luaState, field.c_str());
			lua_gettable(luaState, -2);

			SpeciesBodyPart newPart;
			auto * part = getBodyPart(field);

			newPart.tag = part->tag;

			int qty = 0;
			while (lua_next(luaState, -2) != 0)
			{
				const std::string nf = lua_tostring(luaState, -2);

				if (nf == "qty")
					qty = lua_int();

				else if (nf == "att")
					newPart.attachedTo = lua_str();

				else if (nf == "hit")
					newPart.size = lua_double(); // TODO:

				lua_pop(luaState, 1);
			}

			// Push back Qty # of parts of this type into the new body
			for (int i = 0; i < qty; ++i)
				body.parts.emplace_back(newPart);

			lua_pop(luaState, 1);
		}

		// Attach all parts to the correct places and 
		// make sure order stays identical through sorting vector of parts
		buildBody(body);

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
