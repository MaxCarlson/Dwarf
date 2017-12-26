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

const std::vector<std::string> get_all_item_tags()
{
	std::vector<std::string> itemNames;
	for (auto s : itemDefs)
	{
		itemNames.push_back(s.first);
	}
	return itemNames;
}

void readInItems() noexcept
{
	std::string tag;
	ItemDef c;

	readLuaTable("items",
		[&c, &tag](const auto &key) { tag = key; c = ItemDef{}; c.tag = tag; },
		[&c, &tag](const auto &key) { itemDefs[tag] = c; },
			luaParser{
				{ "name", [&c]() { c.name = lua_str(); } },
				{ "description", [&c]() { c.description = lua_str(); } },
				{ "color", [&c]() { c.color = lua_str(); } },
				//{ "foreground", [&c]() { c.bg = lua_int("foreground"); } },
				{ "glyph", [&c]() { c.charCode = lua_int(); } },
				{ "stackSize", [&c]() { c.stackSize = lua_int(); } },
				{ "tcode", [&c]() { c.tilesetKey = lua_int(); } },
				{ "itemType", [&c]() {
				readLuaInnerT("itemType", [&c](auto type) {
					if (type == "choppingTool") c.categories.set(TOOL_CHOPPING);
					if (type == "diggingTool")  c.categories.set(TOOL_DIGGING);
				});
			}}
		}
	);
}

void sanityCheckItems() noexcept
{
}
