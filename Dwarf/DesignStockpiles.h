#pragma once
#include "ECS\Systems.h"
#include "Coordinates.h"

class DesignStockpiles : public System<Requires<>>
{
public:
	void update(const double duration);

private:
	bool confirm = false;
	Coordinates click = EMPTY_COORDINATES;

	void designStockpiles();
};

