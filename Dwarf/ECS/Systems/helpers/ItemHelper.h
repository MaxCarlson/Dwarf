#pragma once
#include "../../Systems.h"
#include "../../Components/Item.h"
#include "../ECS/Components/PositionComponent.h"
#include "ECS\Components\Claimed.h"

struct ItemDef;
class Item;
struct ReactionInput;

class ItemHelper : public System<Requires<Item, PositionComponent>> // Remove position component later and just test for it!
{
public:
	ItemHelper() = default;
	
	void forEachItem(const std::function<void(Entity)> & func);

	void claim_item(Entity& item);
	void unclaim_item_by_id(std::size_t id);

	void deleteItem(const std::size_t id);

	int get_item_location(std::size_t id);

	template<int T>
	bool isItemCatagoryAvailible()
	{
		for (const auto& i : getEntities())
			if (i.getComponent<Item>().catagory.test(T) && !i.hasComponent<Claimed>())
				return true;

		return false;
	}

	size_t findClosestItemTypeClaimIt(const Entity& e, const int type, const Coordinates& co, bool CLAIM_IT = true);

	int num_reaction_input_items(const ReactionInput& react);
	std::size_t claim_item_by_reaction_inp(const ReactionInput& react);
};

extern ItemHelper itemHelper;




