#pragma once
#include "../../Systems.h"
#include "../../Components/Tags/PlantingTag.h"
#include "../../Components/Seed.h"

class SeedsHelper : public System<Requires<Seed>>
{
public:
	void forAllSeeds(std::function<void(Entity)> func);
};

class FarmingAi : public System<Requires<PlantingTag>>
{
public:
	void init();
	void update(const double duration);

private:
	void doWork(Entity& e, const double& duration);
};

