#pragma once
#include "../../Systems.h"
#include "../../Components/Item.h"
#include "../ECS/Components/PositionComponent.h"

struct ItemDef;
class Item;
struct ReactionInput;

class ItemHelper : public System<Requires<Item, PositionComponent>> 
{
public:
	ItemHelper() = default;
	
	//template<typename T>
	void forEachItem(const std::function<void(Entity)> & func);

	void claim_item(Entity& item);
	void unclaim_item_by_id(std::size_t id);

	void deleteItem(const std::size_t id);

	int get_item_location(std::size_t id);

	int num_reaction_input_items(const ReactionInput& react);
	std::size_t claim_item_by_reaction_inp(const ReactionInput& react);
};

extern ItemHelper itemHelper;




