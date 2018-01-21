#pragma once
#include "../../Systems.h"

class CalenderSystem : public System<Requires<>>
{
public:
	CalenderSystem() = default;

	void update(const double duration);
};

