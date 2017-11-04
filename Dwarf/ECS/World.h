#pragma once

#include "entities.h"

class World
{
public:
	using EntityArray = std::vector<Entity>;

	World();
	
	// Constructs a world with a entity pool
	// the size of entitySizePool
	explicit World(std::size_t entityPoolSize);

	// Add a system type to world
	template<typename TSystem>
	void addSystem(TSystem & system);

	// Removes system of type TSystem
	template<typename TSystem>
	void removeSystem();

	// Does system of type TSystem exist?
	template<typename TSystem>
	bool doesSystemExist() const;

	// Remove all existing systems
	void removeAllSystems();

	// Create a new entity
	Entity createEntity();

	// Create a new entity with an entity template function
	//
	// Template EntityTemplateFunct - type of template function
	// fn - function to apply to entity
	// args - arguments of function
	template< typename EntityTemplateFunct, typename... Args>
	Entity createEntity(EntityTemplateFunct fn, Args&&... args) 
	{
		Entity en = createEntity();
		fn(en, std::forward<Args>(args)...);
		return en;
	}

	// Create multiple entities, returns EntityArray
	EntityArray createEntities(std::size_t number);

	// Deactivates entity and marks for removal
	void killEntity(Entity & entity);

	// Marks multiple entities for removal
	void killEntities(std::vector<Entity>& entities);

	// Mark entity active
	void activateEntity(Entity & entity);

	// Mark an entity for inactivity, not deletion
	void deactivateEntity(Entity & entity);

	// Check if an entity is active
	bool isActivated(Entity & entity) const;

	// Determines if an entity is valid.
	bool isValid(Entity & entity) const;

	// Refreshes world
	void refresh();

	// Removes all systems and all entities
	// from world.
	void clear();

	// Returns a count of entities attached
	// to world. Counts dead and alive entities
	std::size_t getEntityCount() const;
};

