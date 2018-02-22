#include "../stdafx.h"
#include "ItemRead.h"
#include "Lua.h"
#include "Defs\ItemDefs.h"

boost::container::flat_map<std::string, ItemDef> itemDefs;
boost::container::flat_map<std::string, StockpileDef> stockpileDefs;

ItemDef * getItemDef(const std::string tag)
{
	auto itemd = itemDefs.find(tag);

	if (itemd != itemDefs.end())
		return &itemd->second;
	
	return nullptr;
}

StockpileDef * getStockpileDef(const std::string tag)
{
	auto stockd = stockpileDefs.find(tag);

	if (stockd != stockpileDefs.end())
		return &stockd->second; 

	return nullptr;
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


void foreachStockpile(const std::function<void(StockpileDef*)>& func) // Change to iterator if we start modifying the stockpileDefs
{
	for (auto stock : stockpileDefs)
		func(&stock.second);
}

const std::vector<std::pair<std::string, std::string>> get_stockpile_names_tags()
{
	std::vector<std::pair<std::string, std::string>> namesAndTags;

	for (const auto& sdef : stockpileDefs)
	{
		namesAndTags.push_back(std::make_pair(sdef.first, sdef.second.name));
	}

	return namesAndTags;
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
				{ "foreground", [&c]() { c.ch.fg = color_from_name(lua_str().c_str()); } },
				{ "background", [&c]() { c.ch.bg = color_from_name(lua_str().c_str()); } },
				{ "glyph", [&c]() { c.ch.c = lua_int(); } },
				{ "stackSize", [&c]() { c.stackSize = lua_int(); } },
				{ "itemType", [&c]() {
				readLuaInnerT("itemType", [&c](auto type) {
					if (type == "choppingTool") c.categories.set(TOOL_CHOPPING);
					if (type == "diggingTool")  c.categories.set(TOOL_DIGGING);
					if (type == "farmingTool")  c.categories.set(TOOL_FARMING);
					if (type == "food")		    c.categories.set(ITEM_FOOD);
					if (type == "seeds")	    c.categories.set(ITEM_SEED);
					if (type == "soil")			c.categories.set(ITEM_SOIL);
					if (type == "melee")		c.categories.set(ITEM_MELEE_WEAPON);
					if (type == "ranged")		c.categories.set(ITEM_RANGED_WEAPON);
					});
				}},
				{ "stockpile", [&c]() { c.stockpileId = lua_int(); } },
				{ "baseDmg", [&c]() { c.baseDmg = lua_double(); }},
				{ "baseSpeed", [&c]() { c.baseSpeed = lua_double(); }},
				{ "twoHanded", [&c]() { c.twoHanded = true; }}
		}
	);
}

void sanityCheckItems() noexcept
{
}

void readInStockpiles() noexcept
{
	std::string tag;
	std::string name;
	StockpileDef s;

	readLuaTable("stockpiles",
		[&s, &tag](const auto &key) { tag = key; s = StockpileDef{}; s.tag = tag; },
		[&s, &tag](const auto &key) { stockpileDefs[tag] = s; },
			luaParser
			{
				{"name", [&s]() { s.name = lua_str(); } },
				{"id", [&s]() { s.index = lua_int(); } }
			}
		);
}

void sanityCheckStockpiles() noexcept
{
}
