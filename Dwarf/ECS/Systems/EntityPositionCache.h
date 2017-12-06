#pragma once

#include "../Systems.h"
#include "../Components/PositionComponent.h"
#include "../Engine.h"
#include "../World.h"
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

	// This must be done manually with function below
	//void onEntityRemoval(Entity & entity);
};

// Delete entity from position cache
// This must be done manually!
inline void deletePositionCache(const Entity e, Coordinates co)
{
	engine.entityPositionCache->updateEntity<false>(e, EMPTY_COORDINATES, co);
}

