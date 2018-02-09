#pragma once
#include "ECS\Systems.h"
#include "Coordinates.h"

class DesignHarvest : public System<Requires<>>
{
public:
	void init();
	void update(const double duration);

private:

	Coordinates click;
	bool confirm = false;

	void designHarvest();
	void designFarming();
};

