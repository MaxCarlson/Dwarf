#pragma once


extern "C" {
	
#include "lualib.h"
#include "lauxlib.h"
}

//#include "lua.hpp"

extern lua_State* luaState;

void initLua();
void exitLua();


