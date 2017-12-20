#include "Buildings.h"
#include "Defs\BuildingDef.h"
#include "Materials.h"
#include "Lua.h"
#include <boost\container\flat_map.hpp>
#include <iostream>


boost::container::flat_map<std::string, BuildingDef> buildingDefs;

BuildingDef * getBuilding(const std::string & tag)
{
	return &buildingDefs[tag];
}

void readInBuildings() noexcept
{
	lua_getglobal(luaState, "buildings");
	lua_pushnil(luaState);

	std::vector<BuildingDef> tester;

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
			if (field == "structure") def.structure = lua_tonumber(luaState, -1);
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
		tester.push_back(def);
		lua_pop(luaState, 1);
	}
	int a = 5;
}

void sanityCheckBuildings() noexcept
{
}
