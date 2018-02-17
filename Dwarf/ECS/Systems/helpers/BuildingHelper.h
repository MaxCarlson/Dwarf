#pragma once
#include "ECS\Systems.h"
#include "ECS\Components\Building.h"

class BuildingHelper : public System<Requires<Building>>
{
public:
	void forEachBuilding(std::function<void(const Entity&)> func);
	
	template<int type>
	void forEachBuildingOfType(std::function<void(const Entity&)> func)
	{
		forEachBuilding([&func](const Entity& e)
		{
			if (!e.getComponent<Building>().provides.test(type))
				return;

			func(e);
		});
	}
};

extern BuildingHelper buildingHelper;
