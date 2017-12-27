#pragma once

#include "../Systems.h"
#include "../Components/PositionComponent.h"
#include "../Engine.h"
#include "../World.h"
#include "../Map/Tile.h"
#include "../Helpers/PositionCache.h"
#include <iostream>
#include <unordered_map>

extern std::unique_ptr<PositionCache> positionCache; 

class EntityPositionCache : public System<Requires<PositionComponent>> // ~~~ Should we delete this system? As of now it only serves to automatically add entities for us, but not delete them
{
public:
	EntityPositionCache();
	void init();


private:

	void onEntityAdd(Entity & entity);
};

