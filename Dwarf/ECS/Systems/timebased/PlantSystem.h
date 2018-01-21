#pragma once
#include "../../Systems.h"

class PlantSystem : public System<Requires<>>
{
public:
	void update(const double duration);
};

