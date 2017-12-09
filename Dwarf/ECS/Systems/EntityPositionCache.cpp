#include "EntityPositionCache.h"
#include "../Messages/entity_moved_message.h"
#include "../World.h"


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

void EntityPositionCache::onEntityAdd(Entity & entity)
{
	auto co = entity.getComponent<PositionComponent>().co;

	positionCache.emplace(getIdx(co), entity);
}


