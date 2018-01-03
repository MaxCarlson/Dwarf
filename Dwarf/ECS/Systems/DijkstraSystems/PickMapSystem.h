#pragma once

#include "../../Systems.h"
#include "../../Components/PositionComponent.h"
#include "../../Components/Item.h"
#include "../../Components/ItemStored.h"

class PickMapSystem : public System<Requires<PositionComponent, Item>, Excludes<ItemStored>>
{
public:
	PickMapSystem() = default;

	void init();
	void update();

private:
	//DijkstraMapsHandler handler;
};

