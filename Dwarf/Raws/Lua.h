#pragma once
#include <functional>

extern "C" {
	
#include "lualib.h"
#include "lauxlib.h"
}

//#include "lua.hpp"

extern lua_State* luaState;

using luaParser = std::unordered_map<std::string, const std::function<void()>>;

void initLua();
void exitLua();

void readLuaTable(const std::string &table, const std::function<void(std::string)> &onStart, const std::function<void(std::string)> &onEnd, const luaParser & parser);
void readLuaInnerT(const std::string &table, const std::function<void(std::string)> &functor);

inline std::string lua_str() { return lua_tostring(luaState, -1); }
inline int lua_int() { return static_cast<int>(lua_tonumber(luaState, -1)); }
inline double lua_double() { return static_cast<double>(lua_tonumber(luaState, -1)); }


