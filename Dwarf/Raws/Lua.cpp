#include "Lua.h"

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
