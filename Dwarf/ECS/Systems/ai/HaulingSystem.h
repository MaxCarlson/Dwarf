#pragma once
#include "../../Systems.h"
#include "../../Components/Tags/HaulingTag.h"

class HaulingSystem : public System<Requires<HaulingTag>>
{
public:
	HaulingSystem() = default;

	void init();
	void update(const double duration);

private:
	void doWork(Entity e);
};

