#pragma once

#include "../Systems.h"
#include "../Components/PositionComponent.h"
#include "../Engine.h"
#include "../World.h"
#include "../Map/Tile.h"
#include <iostream>
#include <unordered_map>

using region::getIdx;

class EntityPositionCache : public System<Requires<PositionComponent>>
{
public:
	EntityPositionCache();
	void init();
	
	void update();

	// Find and return a vector of Entities 
	// at coordinates co
	std::vector<Entity> findEntities(Coordinates co);

	// Update Entities position in position cache
	//
	// template bool is true when only wishing to remove the entity
	// from the cache
	// Removal is handled automatically when Entity is removed from system
	// (i.e. loses PositionComponent)
	template<bool remove>
	void updateEntity(const Entity e, const Coordinates& newCo, const Coordinates& oldCo);
	
	

private:
	std::unordered_multimap<int, Entity> positionCache;

	void onEntityAdd(Entity & entity);

	// This must be done manually with function below
	//void onEntityRemoval(Entity & entity);
};

template<bool remove>
void EntityPositionCache::updateEntity(const Entity e, const Coordinates & newCo, const Coordinates & oldCo)
{
	// Delete old position for cache if it exists
	auto oldIdx = getIdx(oldCo);

	auto range = positionCache.equal_range(oldIdx);

	for (auto& it = range.first; it != range.second; ++it)
	{
		// If the Entities match by eid.index 
		// ( doesn't check counter, maybe it should? )
		if (it->second.getId().index == e.getId().index)
		{
			// Erase old position entry
			// Possibly this should be a move operation
			// for non deleting ops? or maybe just emplace_hint?
			positionCache.erase(it);

			// If we're updating the entity
			// move Entity to new position in cache
			if (!remove)
				positionCache.emplace(getIdx(newCo), e);

			return;
		}

	}

	std::cout << "Entity Position Cache Issue!!" << std::endl;
}

// Delete entity from position cache
// This must be done manually!
inline void deletePositionCache(const Entity e, Coordinates co)
{
	engine.entityPositionCache->updateEntity<true>(e, EMPTY_COORDINATES, co);
}

