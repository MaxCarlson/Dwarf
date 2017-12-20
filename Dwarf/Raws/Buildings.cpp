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
			if (field == "structure") def.structure = true;
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
					}
				}

			}
		}
	}
}

void sanityCheckBuildings() noexcept
{
}
