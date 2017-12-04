#pragma once

#include "../../Systems.h"
#include "../../Components/PositionComponent.h"
#include "../../Components/Item.h"


class PickMapSystem : public System<Requires<PositionComponent, Item>>
{
public:
	PickMapSystem() = default;

	void init();
	void update();

private:
	//DijkstraMapsHandler handler;
};

