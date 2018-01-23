#pragma once
#include <functional>
#include <unordered_map>

extern "C" {
	
#include "lualib.h"
#include "lauxlib.h"
}

//#include "lua.hpp"

extern lua_State* luaState;

void initLua();
void exitLua();

struct LuaLife
{
	LuaLife() { initLua(); }
	~LuaLife() { exitLua(); }
};

using luaParser = std::unordered_map<std::string, const std::function<void()>>;


void loadLuaScript(std::string fileName);
void readLuaTable(const std::string &table, const std::function<void(std::string)> &onStart, const std::function<void(std::string)> &onEnd, const luaParser & parser);
void readLuaInnerT(const std::string table, const std::function<void(std::string)> &functor);
void readLuaSepT(const std::string table, const std::function<void(std::string)> &functor);

void readLuaTable2D(const std::string &table, const std::function<void(std::string)> &functor1, const std::function<void(std::string)> &functor2);

void readLuaMultiTable(const std::string &table, const std::function<void()> &functor1, const std::function<void(std::string)> &functor2, const std::function<void()> &functor3);

inline std::string lua_str() { return lua_tostring(luaState, -1); }
inline int lua_int() { return static_cast<int>(lua_tonumber(luaState, -1)); }
inline double lua_double() { return static_cast<double>(lua_tonumber(luaState, -1)); }


