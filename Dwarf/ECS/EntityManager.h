#pragma once

#include "Component.h"
#include "Entities.h"

#include <memory>
#include <bitset>
#include <algorithm>
#include <array>
#include <vector>
#include <type_traits>
#include <cereal.hpp>
#include "../cereal/types/vector.hpp"
#include "../cereal/types/array.hpp"
#include "../cereal/types/bitset.hpp"
#include "../cereal/types/memory.hpp"


class EntityIdPool
{
public:
	explicit EntityIdPool(std::size_t poolSize);

	// Creates and returns newly ceated
	// Entity Id
	Entity::Id create();

	// Remove entity id from free pool
	void remove(Entity::Id id);

	// Get entity id at index index
	Entity::Id get(std::size_t index) const;

	// Returns whether or not id is a valid
	// Entity Id
	bool isValid(Entity::Id id) const;

	// Returns capacity of EntityIdPool
	std::size_t getSize() const;

	// Resized this to size amount
	void resize(std::size_t amount);

	// Clears all entity Id's
	// Will invalidate every entity Id given out so far
	void clear();

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(counts, freeIdList, nextId, defaultPoolSize);
	}

	template<class Archive>
	void load(Archive & iarchive)
	{
		iarchive(counts, freeIdList, nextId, defaultPoolSize);
	}

private:

	// Starting size of the entity pool
	std::size_t defaultPoolSize;

	// Next Id that will be used if Id
	// is requested
	std::uint64_t nextId;

	// Free entity Id's
	std::vector<Entity::Id> freeIdList;

	// Counts for how many times a particular
	// Entity id has be re-used. Avoids removing an entity
	// only to have it try and be used in something before refresh
	std::vector<std::uint32_t> counts;
};

// Make this new entity manager
class EntityManager
{
public:
	explicit EntityManager(std::size_t entityAmount);

	// Add componenet to entity
	void addComponent(Entity & entity, Component* component, TypeId componentTypeId);

	// Remove componenet of entity
	void removeComponent(Entity & entity, TypeId componentTypeId);

	// Removes all components of entity
	void removeAllComponents(Entity & entity);

	// Get a refrence to a component of an entity if it exists
	Component & getComponent(const Entity & entity, TypeId componentTypeId) const;

	// Returns a bitset of current components this Entity has
	ComponentTypeList getComponentTypeList(const Entity & entity) const;

	// Returns an array of all components this Entity has
	ComponentArray getComponents(const Entity & entity) const;

	// Does Entity entity have component type componentTypeId ?
	bool hasComponent(const Entity & entity, TypeId componentTypeId) const;

	void resize(std::size_t entityAmount);

	void clear();

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(componentEntries);
	}

	template<class Archive>
	void load(Archive & iarchive)
	{
		iarchive(componentEntries);
	}

private:
	typedef std::array<std::unique_ptr<Component>, MAX_COMPONENTS> ImplComponentArray;

	struct EntityComponents
	{
		EntityComponents() = default;
		
		EntityComponents(EntityComponents&& e) :
			components(std::move(e.components)),
			componentTypeList(std::move(e.componentTypeList)) {}

		// An array of every component an entity has
		// Index is by component ID 
		ImplComponentArray components;

		// A bitset denoting which components this entity has
		// Used for fast system component checking.
		// Components bits are set based on order of first component
		// generation.
		ComponentTypeList componentTypeList;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(components, componentTypeList);
		}
	};

	// All components for every entity. Indexed by entity ID
	std::vector<EntityComponents> componentEntries;

	ImplComponentArray& getComponentsArrayImpl(const Entity& en);
	const ImplComponentArray& getComponentsArrayImpl(const Entity& en) const;
};
