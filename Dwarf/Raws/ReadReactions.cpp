#include "../stdafx.h"
#include "ReadReactions.h"
#include "Lua.h"

boost::container::flat_map<std::string, Reaction> reactions;
boost::container::flat_map<std::string, std::vector<Reaction>> reactionsByBuilding;

Reaction * getReaction(const std::string tag) 
{
	auto f = reactions.find(tag);

	if (f != reactions.end())
		return &f->second;

	return nullptr;
}

const std::vector<std::string> getReactionsByWorkshop(const std::string tag)
{
	auto f = reactionsByBuilding.find(tag);

	std::vector<std::string> names;

	if (f != reactionsByBuilding.end())
	{
		for (const auto& b : f->second)
		{
			names.push_back(b.tag);
		}
	}


	return names;
}

void readInReactions() noexcept
{
	Reaction r;
	ReactionInput inp;
	std::pair<std::string, int> out;
	std::string tag;

	readLuaTable("reactions", 
		[&r, &tag](const auto& key) { tag = key; r = Reaction{}; r.tag = tag; },
		[&r, &tag](const auto& key) { reactions[tag] = r; },
		luaParser
		{
			{ "name", [&r]() { r.name = lua_str(); } },
			{ "workshop", [&r]() { r.workshop = lua_str(); } },
			{ "skill", [&r]() { r.skill = lua_str(); } },
			{ "difficulty", [&r]() { r.difficulty = lua_int(); } },
			{ "inputs",[&r, &inp]() {
				readLuaMultiTable("inputs",
					[&inp]() { inp = ReactionInput{}; },
					[&inp](auto type) 
					{
						if (type == "item")     inp.tag = lua_str();
						if (type == "qty")	    inp.quantity = lua_int();
						if (type == "material") inp.req_material = static_cast<std::size_t>(lua_int());
						if (type == "mat_type")
						{
							auto ss = lua_str();

							if (ss == "rock")
								inp.req_material_type = spawn_type_rock;
							else if (ss == "soil")
								inp.req_material_type = spawn_type_soil;
							else if (ss == "sand")
								inp.req_material_type = spawn_type_sand;
							else if (ss == "metal")
								inp.req_material_type = spawn_type_metal;
							else if (ss == "synthetic")
								inp.req_material_type = spawn_type_synthetic;
							else if (ss == "organic")
								inp.req_material_type = spawn_type_organic;
						}
					},
					[&r, &inp]() { r.inputs.push_back(inp); }
				);
			}},
			{ "outputs",[&r, &out]() {
				readLuaMultiTable("outputs",
					[&out]() { out = std::make_pair("", 0); },
					[&out](auto type) 
					{
						if (type == "item") out.first  = lua_str();
						if (type == "qty")  out.second = lua_int();
				    },
					[&r, &out]() { r.outputs.push_back(out); }
				);
			}},
			{ "time", [&r]() { r.time = lua_double() * 1000.0; } },
		}
	);

	// Add reactions by building to container
	for (const auto& re : reactions)
		reactionsByBuilding[re.second.workshop].push_back(re.second);
}

void sanityCheckReactions()
{
}
