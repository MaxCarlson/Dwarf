#include "ReadReactions.h"
#include "Lua.h"
#include <boost\container\flat_map.hpp>

boost::container::flat_map<std::string, Reaction> reactions;

Reaction * getReaction(std::string tag)
{
	auto f = reactions.find(tag);

	if (f != reactions.end())
		return &f->second;

	return nullptr;
}

const std::vector<Reaction*> getReactionsByWorkshop(std::string tag)
{
	return std::vector<Reaction*>();
}

void readInReactions() noexcept
{
	Reaction r;
	ReactionInput inp;
	std::vector<std::pair<std::string, int>> out;
	std::string tag;

	readLuaTable("reactions", 
		[&r, &tag](const auto& key) { tag = key; r = Reaction{}; r.tag = tag; },
		[&r, &tag](const auto& key) { reactions[tag] = r; },
		luaParser
		{
			{ "name", [&r]() { r.name = lua_str(); }},
			{ "workshop", [&r]() { r.workshop = lua_str(); }},
			{ "skill", [&r]() { r.skill = lua_str(); } },
			{ "inputs",[&r, &inp]() {
				readLuaTable2D("inputs",
					[&inp](auto type1) { inp = ReactionInput{}; },
					[&inp](auto type2) {
						if (type2 == "item") inp.tag = lua_str();
						if (type2 == "qty") inp.quantity = lua_int();
					}
				);
			
			}},
			{ "inputs",[&r, &inp]() {
				readLuaTable2D("inputs",
					[](auto type1) {},
					[&inp](auto type2) {
						if (type2 == "item") inp.tag = lua_str();
						if (type2 == "qty") inp.quantity = lua_int();
					}
				);
			}}
		}

	);
	int a = 54;
}

void sanityCheckReactions()
{
}
