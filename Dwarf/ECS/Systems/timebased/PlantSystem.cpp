#include "stdafx.h"
#include "PlantSystem.h"
#include "Map\Tile.h"
#include "Raws\ReadPlants.h"
#include "Raws\Defs\PlantDef.h"
#include "Globals\global_calender.h"


void PlantSystem::update(const double duration)
{
	if (dayPassed)
	{
		for(int x = 0; x < MAP_WIDTH - 1; ++x)
			for(int y = 0; y < MAP_HEIGHT - 1; ++y)
				for (int z = 10; z < MAP_DEPTH - 1; ++z)
				{
					const int idx = getIdx({ x, y, z });

					if (region::plantType(idx) > 0)
					{
						auto ptick  = region::plantTicker(idx) + 1;
						auto pcycle = region::plantLifeCycle(idx);

						auto plant = getPlantDef(region::plantType(idx));

						if (ptick > plant->lifecycle[pcycle])
						{
							++pcycle;
							ptick = 0;

							if (pcycle > 3)
							{
								pcycle = 0;
								// TODO: destory plant and spread it if it's non annual
							}


							region::setPlantLifecycle(idx, pcycle);
						}
						region::setPlantTicker(idx, ptick);
					}
				}
	}
}
