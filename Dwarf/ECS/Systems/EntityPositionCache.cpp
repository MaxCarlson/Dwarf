#include "EntityPositionCache.h"
#include "../Messages/entity_moved_message.h"
#include "../World.h"


using namespace region;

std::unique_ptr<PositionCache> positionCache;

EntityPositionCache::EntityPositionCache()
{
	
}

void EntityPositionCache::init()
{
	subscribe<entity_moved_message>([this](entity_moved_message &msg)
	{
		positionCache->removeNode({ msg.oldCo, msg.e.getId().index });
		positionCache->addNode({ msg.newCo, msg.e.getId().index });
	});
}

void EntityPositionCache::onEntityAdd(Entity & entity)
{
	auto co = entity.getComponent<PositionComponent>().co;

	positionCache->addNode({ co, entity.getId().index });
}


