#pragma once
#include "ECS\Systems.h"

class DesignBuilding : public System<Requires<>>
{
public:
	DesignBuilding() = default;

	void init();
	void update(const double duration);

private:
	void drawPossibleBuilding(const std::string &tag);
	void workshopTab();
};

