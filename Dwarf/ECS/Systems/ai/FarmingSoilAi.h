#pragma once
#include "ECS\Systems.h"
#include "ECS\Components\Tags\FarmSoilTag.h"

class FarmingSoilAi : public System<Requires<FarmSoilTag>>
{
public:
	void init();
	void update(const double duration);

private:

};

