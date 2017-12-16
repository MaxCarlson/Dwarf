#include "ItemRead.h"
#include "Lua.h"
#include "Defs\ItemDefs.h"
#include <boost\container\flat_map.hpp>

boost::container::flat_map<std::string, ItemDef> itemDefs;

ItemDef * getItemDef(const std::string tag)
{
	auto itemd = itemDefs.find(tag);
	if (itemd == itemDefs.end())
		return nullptr;
	
	return &itemd->second;
}

void readInItems() noexcept
{

	std::string tag;
	ItemDef c;

	readLuaTable("items_tools",
		[&c, &tag](const auto &key) { tag = key; c = ItemDef{}; c.tag = tag; },
		[&c, &tag](const auto &key) { itemDefs[tag] = c; },
			luaParser{
				{ "name", [&c]() { c.name = lua_str(); } },
				{ "description", [&c]() { c.description = lua_str(); } },
				{ "color", [&c]() { c.color = lua_int(); } },
				//{ "foreground", [&c]() { c.bg = lua_int("foreground"); } },
				{ "glyph", [&c]() { c.charCode = lua_int(); } },
				{ "stack_size", [&c]() { c.stackSize = lua_int(); } },
				{ "itemtype", [&c]() {
				readLuaInnerT("itemtype", [&c](auto type) {
					if (type == "chopping tool") c.categories.set(TOOL_CHOPPING);
					if (type == "digging tool")  c.categories.set(TOOL_DIGGING);
				});
			}}
		}
	);
}

void sanityCheckItems() noexcept
{
}
