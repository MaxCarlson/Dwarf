#pragma once
#include "ECS\Systems.h"
#include "ECS\Components\Tags\FarmSoilTag.h"

class FarmingSoilAi : public System<Requires<FarmSoilTag>>
{
public:
	void init();
	void update(const double duration);

private:
	void testTool(const Entity& e, FarmSoilTag &tag);
	void findFarm(const Entity& e, const Coordinates& co, WorkTemplate<FarmSoilTag> &work, FarmSoilTag &tag);
	void findHoe(const Entity& e, const Coordinates& co, WorkTemplate<FarmSoilTag> &work, FarmSoilTag &tag);
	void findSoil(const Entity& e, const Coordinates& co, WorkTemplate<FarmSoilTag> &work, FarmSoilTag &tag);
};

