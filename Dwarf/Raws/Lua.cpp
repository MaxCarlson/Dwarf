#include "Lua.h"
#include <iostream>

lua_State* luaState;

void initLua()
{
	luaState = luaL_newstate();
	
	static const luaL_Reg luaLibs[] =
	{
		{ "base", luaopen_base },
		{ NULL, NULL }
	};

	const luaL_Reg *lib = luaLibs;
	for (; lib->func != NULL; lib++)
	{
		lib->func(luaState);
		lua_settop(luaState, 0);
	}
}

void exitLua()
{
	lua_close(luaState);
}


inline void callFunctorkey(const luaParser & parser, const std::string& field, const std::string& table)
{
	auto finder = parser.find(field);
	if (finder != parser.end()) 
	{
		const auto functor = finder->second;
		functor();
	}
	else {
		throw std::runtime_error(std::string("Lua - no known parser for [") + field + std::string("] in table: " + table));
	}
}

void loadLuaScript(std::string fileName)
{
	if (luaL_loadfile(luaState, fileName.c_str()) || lua_pcall(luaState, 0, 0, 0))
	{
		std::cout << lua_tostring(luaState, -1) << std::endl;
	}
}

void readLuaTable(const std::string & table, const std::function<void(std::string)>& onStart, const std::function<void(std::string)>& onEnd, const luaParser & parser)
{
	lua_getglobal(luaState, table.c_str());
	lua_pushnil(luaState);

	while (lua_next(luaState, -2) != 0)
	{
		const std::string key = lua_tostring(luaState, -2);

		onStart(key);
		lua_pushstring(luaState, key.c_str());
		lua_gettable(luaState, -2);

		while (lua_next(luaState, -2) != 0)
		{
			const std::string field = lua_tostring(luaState, -2);
			callFunctorkey(parser, field, table);
			lua_pop(luaState, 1);
		}
		onEnd(key);

		lua_pop(luaState, 1);
	}
}

void readLuaInnerT(const std::string & table, const std::function<void(std::string)>& functor)
{
	lua_pushstring(luaState, table.c_str());
	lua_gettable(luaState, -2);

	while (lua_next(luaState, -2) != 0)
	{
		const std::string s = lua_tostring(luaState, -1);
		functor(s);
		lua_pop(luaState, 1);
	}
}

void readLuaTable2D(const std::string & table, const std::function<void(std::string)>& functor1, const std::function<void(std::string)>& functor2)
{
	lua_pushstring(luaState, table.c_str());
	lua_gettable(luaState, -2);

	//while (lua_next(luaState, -2) != 0)
	//{

	lua_next(luaState, -2);

	const std::string s = lua_tostring(luaState, -2);
	functor1(s);

	lua_pushnil(luaState);
	lua_gettable(luaState, -2);

	if (table == "render") 
		int a = 5;

	while (lua_next(luaState, -2) != 0)
	{
		std::string cf = lua_tostring(luaState, -2);
		functor2(cf);

		lua_pop(luaState, 1);
	}

	lua_pop(luaState, 1);
	lua_pop(luaState, 1);

	//}
}
