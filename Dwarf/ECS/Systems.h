#pragma once
#include "EntityManager.h"
#include "World.h"
#include "Filter.h"
#include <vector>


class SystemBase
{
public:

	SystemBase(const Filter & filter) : world(nullptr), filter(filter) {};

	virtual ~SystemBase() = 0;

	// Returns a Filter object that holds two
	// bool vectors that describe required and 
	// excluded components
	const Filter& getFilter() const { return filter; }

	// Returns refrernce to world
	// this system is associated with
	World& getWorld() const { return *world; }


	// Returns all entitys of a system
	const std::vector<Entity>& getEntities() const { return entities; }

private:

	virtual void initialize() {};

	virtual void onEntityAdd(Entity & entity) {};

	virtual void onEntityRemoval(Entity & entity) {};

	void addEntity(Entity & entity)
	{
		entities.push_back(entity);
		onEntityAdd(entity);
	}

	void remove(Entity & entity)
	{
		entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
		onEntityRemoval(entity);
	}

	void setWorld(World & setWorld)
	{
		world = &setWorld;
		initialize();
	}

	World * world;

	// The component filter // Bitset to & against components of entitys to test if they have correct components for system
	Filter filter;

	// All entities that this system should care about
	// or that are attached
	std::vector<Entity> entities;

	friend World;
};

template<class RequireList, class ExcludeList = Excludes<>>
class System : public SystemBase
{
public:
	System() : SystemBase{ MakeFilter<RequireList, ExcludeList>() } {};

};

// Return a unique system ID if system is
// being generated/add for first time.
// Otherwise return the ID associated with
// system of type T
template<class T>
TypeId SystemTypeId()
{
	return ClassTypeId<SystemBase>::getTypeId<T>();
}