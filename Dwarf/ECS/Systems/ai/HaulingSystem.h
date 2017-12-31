#pragma once
#include "../../Systems.h"

class HaulingSystem : public System<Requires<>>
{
public:
	HaulingSystem() = default;

	void init();
	void update(const double duration);
};

