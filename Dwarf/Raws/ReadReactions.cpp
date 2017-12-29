#include "ReadReactions.h"
#include "Lua.h"
#include <boost\container\flat_map.hpp>

boost::container::flat_map<std::string, Reaction> reactions;
boost::container::flat_map<std::string, std::vector<Reaction>> reactionsByBuilding;

Reaction * getReaction(const std::string tag) 
{
	auto f = reactions.find(tag);

	if (f != reactions.end())
		return &f->second;

	return nullptr;
}

const std::vector<Reaction>* getReactionsByWorkshop(const std::string tag)
{
	auto f = reactionsByBuilding.find(tag);

	if (f != reactionsByBuilding.end())
		return &f->second;

	return nullptr;
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
				readLuaMultiTable("inputs",
					[&inp]() { inp = ReactionInput{}; },
					[&inp](auto type2) {
						if (type2 == "item") inp.tag = lua_str();
						if (type2 == "qty") inp.quantity = lua_int();
					},
					[&r, &inp]() { r.inputs.push_back(inp); }
				);
			}}
		}
	);
	int a = 54;
}

void sanityCheckReactions()
{
}
