#pragma once

#include "entities.h"
#include "Systems.h"
#include "EntityManager.h"

#include <unordered_map>
#include <memory>

class World
{
private:

	struct SystemDeleter
	{
		void operator() (SystemBase * system) const;
	};

	// Index of map is the ID og system, generated by SystemTypeId<class t>()
	// Allows only one of each system to be stored in world
	using SystemArray = std::unordered_map<TypeId, std::unique_ptr<SystemBase, SystemDeleter>>;

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
	bool isActivated(const Entity & entity) const;

	// Determines if an entity is valid.
	bool isValid(const Entity & entity) const;

	// Refreshes world
	void refresh();

	// Removes all systems and all entities
	// from world.
	void clear();

	// Returns a count of entities attached
	// to world. Counts dead and alive entities
	std::size_t getEntityCount() const;

	// Return all entities in world
	const EntityArray& getAllEntities() const;

	// Returns entity and index of idx
	Entity getEntity(std::size_t idx);

private:

	SystemArray systems;

	// Class used for accessing new and invalidated id's
	// recycles old id's
	EntityIdPool entityIdPool;

	struct EntityAttributes
	{
		struct Attributes
		{
			// Denoting whether entity is active
			bool activated;

			// A bitset that can be &'d against to test
			// whether entity has a particual systsem
			// or set of systems
			std::vector<bool> systems;
		};

		explicit EntityAttributes(std::size_t amountOfEntities) 
			: entityManager(amountOfEntities), attributes(amountOfEntities) {};

		// Mainly storage container for entities
		// and their components. Provides some helpers
		EntityManager entityManager;

		// Attributes of each and every entity
		std::vector<Attributes> attributes;

		void resize(std::size_t amountOfEntities)
		{
			entityManager.resize(amountOfEntities);
			attributes.resize(amountOfEntities);
		}

		void clear() 
		{
			entityManager.clear();
			attributes.clear();
		}
	};

	// Holds details about whether an entity is active
	// and which systems said Entity holds
	EntityAttributes entityAttributes;

	struct EntityCache
	{
		// Contains all live Entities
		EntityArray alive;

		// Holds killed Entities, 
		// get's cleared everry refresh call
		EntityArray killed;

		// Holds just activated Entities
		// get's cleared on refresh call
		EntityArray activated;

		// Holds just deactivated Entities
		// get's cleared on refresh call
		EntityArray deactivated;

		// Clears temporary cache
		void clearTemp()
		{
			killed.clear();
			activated.clear();
			deactivated.clear();
		}

		// Clears all cache
		void clear()
		{
			alive.clear();
			clearTemp();
		}
	};

	EntityCache entityCache;

	// Size helper functions
	void checkForResize(std::size_t amountOfEntitiesToAllocate);
	void resize(std::size_t amount);

	// System helper functions
	void addSystem(SystemBase& system, TypeId systemTypeId);
	void removeSystem(TypeId systemTypeId);
	bool doesSystemExist(TypeId systemTypeId) const;

	friend class Entity;
};

template<typename TSystem>
inline void World::addSystem(TSystem & system)
{
	addSystem(system, SystemTypeId<TSystem>());
}

template<typename TSystem>
inline void World::removeSystem()
{
	removeSystem(SystemTypeId<TSystem>());
}

template<typename TSystem>
inline bool World::doesSystemExist() const
{
	return doesSystemExist(SystemTypeId<TSystem>());
}

// Add does system exist in this world function?
