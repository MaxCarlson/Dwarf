#include "EntityPositionCache.h"
#include "../Messages/entity_moved_message.h"
#include "../Map/Tile.h"
#include "../World.h"

#include <iostream>

using namespace region;

EntityPositionCache::EntityPositionCache()
{
	positionCache.reserve(15000); // Look into sizing?
}

void EntityPositionCache::init()
{
	subscribe<entity_moved_message>([this](entity_moved_message &msg)
	{
		updateEntity<false>(msg.e, msg.newCo, msg.oldCo);
	});
}

void EntityPositionCache::update()
{
}

std::vector<Entity> EntityPositionCache::findEntities(Coordinates co)
{
	std::vector<Entity> entities;
	auto idx = getIdx(co);

	auto range = positionCache.equal_range(idx);

	for (auto& it = range.first; it != range.second; ++it)
	{
		entities.emplace_back(it->second);
	}

	return entities;
}

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
			if(!remove)
				positionCache.emplace(getIdx(newCo), e);

			return;
		}
			
	}

	std::cout << "Entity Position Cache Issue!!" << std::endl;
}

void EntityPositionCache::onEntityAdd(Entity & entity)
{
	auto co = entity.getComponent<PositionComponent>().co;

	positionCache.emplace(getIdx(co), entity);
}


