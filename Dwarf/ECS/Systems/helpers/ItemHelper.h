#pragma once
#include "../../Systems.h"
#include "../../Components/Item.h"

struct ItemDef;
class Item;

class ItemHelper : public System<Requires<Item>> // Excludes<ItemStored> ??
{
public:
	ItemHelper() = default;
	
	//template<typename T>
	void forEachItem(const std::function<void(Entity)> & func);
};

extern ItemHelper itemHelper;

//bool item_match_type(ItemDef d, Item i);

