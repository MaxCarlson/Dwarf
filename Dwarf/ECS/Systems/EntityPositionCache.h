#pragma once

#include "../Systems.h"
#include "../Components/PositionComponent.h"
#include <unordered_map>

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
	void onEntityRemoval(Entity & entity);
};

