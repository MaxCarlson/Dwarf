#pragma once
#include "../../Systems.h"
#include "../../Components/Item.h"

struct ItemDef;
class Item;
struct ReactionInput;

class ItemHelper : public System<Requires<Item>> // Excludes<ItemStored> ??
{
public:
	ItemHelper() = default;
	
	//template<typename T>
	void forEachItem(const std::function<void(Entity)> & func);

	int num_reaction_input_items(const ReactionInput& react);
	std::size_t claim_item_by_reaction_inp(const ReactionInput& react);
};

extern ItemHelper itemHelper;


