#include "Buildings.h"
#include "Defs\BuildingDef.h"
#include "Materials.h"
#include "Lua.h"
#include <boost\container\flat_map.hpp>
#include <iostream>


boost::container::flat_map<std::string, BuildingDef> buildingDefs;

BuildingDef * getBuilding(const std::string & tag)
{
	auto find = buildingDefs.find(tag);
	if (find != buildingDefs.end())
		return &find->second;

	return nullptr;
}

void vecCharIdx(BuildingDef & b, int idx, uint16_t c)
{
	if (b.charCodes.size() < idx)
		b.charCodes.resize(idx);

	b.charCodes[idx - 1] = c;
}

void readInBuildings() noexcept
{
	//lua_getglobal(luaState, "buildings");
	//lua_pushnil(luaState);

	//std::vector<BuildingDef> tester;

	BuildingDef b;
	std::string tag;
	ReactionInput inp;

	readLuaTable("buildings", 
		[&b, &tag](const auto& key) { tag = key; b = BuildingDef{}; b.tag = tag; },
		[&b, &tag](const auto& key) { buildingDefs[tag] = b; },
		luaParser
		{
			{ "name", [&b]() {b.name = lua_str(); }},
			{ "description", [&b]() { b.description = lua_str(); }},
			{ "width", [&b]() { b.width = lua_int(); }},
			{ "height", [&b]() { b.height = lua_int(); } },
			{ "structure", [&b]() { b.structure = true; } },
			{ "components", [&b, &inp]() {
				readLuaTable2D("components",
					[&b, &inp](auto type1) { inp = ReactionInput{}; },
					[&b, &inp](auto type2) {
						if (type2 == "item") inp.tag = lua_str();
						if (type2 == "qty") inp.quantity = lua_int();
					}

				);
				b.components.push_back(inp);
			}},
			{"provides", [&b]() {
				readLuaTable2D("provides",
					[  ](auto type1) {},
					[&b](auto type2) {
						if (type2 == "wall")  b.provides.push_back(BuildingProvides{  provides_wall  });
						if (type2 == "floor") b.provides.push_back(BuildingProvides{  provides_floor });
						if (type2 == "ramp")  b.provides.push_back(BuildingProvides{  provides_ramp  });
					} 

				);
				}		
			},	
			{"skill", [&b]() { // Not implemented yet
				readLuaTable2D("skill",
					[&b](auto type1) {},
					[&b](auto type2) {
						if (type2 == "name") b.skill_required.first = lua_str();
						if (type2 == "difficulty") b.skill_required.second = lua_int();
					}
				);
				}
			},
			{ "render", [&b]() { 
				readLuaTable2D("render",
					[  ](auto type1) {},
					[&b](auto type2) {
						if (type2 == "glyph")  vecCharIdx(b, 1, lua_int()); // Figure out how to integrate the luaState named chars
						if (type2 == "glyph1") vecCharIdx(b, 2, lua_int());
						if (type2 == "glyph2") vecCharIdx(b, 3, lua_int());
						if (type2 == "glyph3") vecCharIdx(b, 4, lua_int());
					}
				);
				}
			},
		}
	);
	/*
	// Loop through all buildings
	while (lua_next(luaState, -2) != 0)
	{
		BuildingDef def;

		std::string key = lua_tostring(luaState, -2);
		def.tag = key;

		lua_pushstring(luaState, key.c_str());
		lua_gettable(luaState, -2);

		// Loop through all of a buildings fields
		while (lua_next(luaState, -2) != 0)
		{
			std::string field = lua_tostring(luaState, -2);

			if (field == "name") def.name = lua_tostring(luaState, -1);
			if (field == "description") def.description = lua_tostring(luaState, -1);
			if (field == "structure") def.structure = lua_toboolean(luaState, -1);
			if (field == "width") def.width = lua_tonumber(luaState, -1);
			if (field == "height") def.height = lua_tonumber(luaState, -1);
			
			if (field == "components")
			{
				lua_pushstring(luaState, field.c_str());
				lua_gettable(luaState, -2);

				// Loop through all of a buildings components
				while (lua_next(luaState, -2) != 0)
				{
					lua_pushnil(luaState);
					lua_gettable(luaState, -2);

					ReactionInput inp;
					
					while (lua_next(luaState, -2) != 0)
					{
						std::string cf = lua_tostring(luaState, -2);

						if (cf == "item") inp.tag = lua_tostring(luaState, -1);
						if (cf == "qty") inp.quantity = lua_tonumber(luaState, -1);
						if (cf == "material")
						{
							std::string matName = lua_tostring(luaState, -1);
							auto mat = getMaterialIdx(matName);
							if (!mat)
								std::cout << "Not a valid material!!" << "\n";
							else
								inp.req_material = mat;
						}

						//if(cf == "matType") ~ not implemented

						lua_pop(luaState, 1);
					}

					def.components.push_back(inp);
					lua_pop(luaState, 1);
				}
			}
			

			//if(field == "skill") // Not implemented yet
			
			if (field == "provides")
			{
				lua_pushstring(luaState, field.c_str());
				lua_gettable(luaState, -2);

				while (lua_next(luaState, -2) != 0)
				{
					BuildingProvides provides;
					std::string t = lua_tostring(luaState, -2);

					if (t == "wall") provides.provides = provides_wall; // Add more of these once there is a need
					if (t == "floor") provides.provides = provides_floor;
					if (t == "ramp") provides.provides = provides_ramp;

					lua_pushstring(luaState, field.c_str());
					lua_gettable(luaState, -2);

					while (lua_next(luaState, -2) != 0)
					{
						std::string pvars = lua_tostring(luaState, -2);

						//if(pvars == "radius") // Not implemented yet!
						lua_pop(luaState, 1);
					}

					def.provides.push_back(provides);
					lua_pop(luaState, 1);
				}
			}

			if (field == "render")
			{
				lua_pushstring(luaState, field.c_str());
				lua_gettable(luaState, -2);

				while (lua_next(luaState, -2) != 0)
				{
					std::string type = lua_tostring(luaState, -2);

					if(type == "glyph") def.charCodes.push_back(static_cast<uint16_t>(lua_tonumber(luaState, -1)));

					lua_pop(luaState, 1);
				}
			}
			
			lua_pop(luaState, 1);
		}
		buildingDefs[key] = def;
		//tester.push_back(def);
		lua_pop(luaState, 1);
	}
	*/
}

void sanityCheckBuildings() noexcept
{
}
