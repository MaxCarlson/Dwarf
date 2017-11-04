#pragma once

#include "Component.h"

#include <memory>
#include <bitset>
#include <algorithm>
#include <array>
#include <vector>
#include <type_traits>




// Max components entity can hold
constexpr std::size_t MAX_COMPONENTS = 64;

using ComponentTypeList = std::bitset<MAX_COMPONENTS>;

// Make this new entity manager
class EntityManager
{
public:
	void addComponent(Entity & entity) {
		//auto index = entity.getId();
	}

private:
	typedef std::array<std::unique_ptr<Component>, MAX_COMPONENTS> ComponentArray;

	struct EntityComponents
	{
		EntityComponents() = default;
		
		EntityComponents(EntityComponents&& e) :
			components(std::move(e.components)),
			componentTypeList(std::move(e.componentTypeList)) {}

		// An array of every component an entity has
		// Index is by component ID 
		ComponentArray components;

		// A bitset denoting which components this entity has
		// Used for fast system component checking.
		// Components bits are set based on order of first component
		// generation.
		ComponentTypeList componentTypeList;
	};

	// All components for every entity. Indexed by entity ID
	std::vector<EntityComponents> componentEntries;
};

/*
using ComponentID = std::size_t;

// Component ID's by order of initialization
inline ComponentID getUniqueComponentID() noexcept
{
static ComponentID lastID = 0;
return ++lastID;
}

// Returns component ID with input of component class
template <typename T> inline ComponentID getComponentTypeID() noexcept
{
static ComponentID typeID = getUniqueComponentID();
return typeID;
}


// Used for matching entities to component sets
using ComponentBitSet = std::bitset<MAX_COMPONENTS>;
using ComponentArray  = std::array<Component*, MAX_COMPONENTS>;

// Groups are used for fast access to entities
// that have a particular component or other "grouping"
using Group = std::size_t;
constexpr std::size_t maxGroups{ 64 };
using GroupBitset = std::bitset<maxGroups>;

enum Groups : size_t
{
THINGS_WITH_RENDER_G,
CREATURES_G
};

class Component
{
public:
Entity* entity;

virtual void init() {};
virtual void update() {};
virtual void draw() {};
virtual ~Component() {};
};

class Entity
{
private:
bool active = true;
std::vector<std::unique_ptr<Component>> components;

ComponentArray   componentArray;
ComponentBitSet componentBitSet;
GroupBitset         groupBitset;

EntityManager & manager;

public:
Entity(EntityManager & manager) : manager(manager) {}

void update()
{
for (auto& co : components) co->update();
}
void draw()
{
for (auto& co : components) co->draw();
}

// Does entity still exist in gameworld?
bool isActive() const { return active;  }
// Tag entity for removal
void destroy() { active = false; }

// Asks if entity has component type T
template <typename T> bool hasComponent() const
{
return componentBitSet[getComponentTypeID<T>()];
}

template <typename T, typename... TArgs>
T& addComponent(TArgs&&... mArgs)
{
// Add component to component vector
// passing any arguments to constructers we may need
T* c(new T(std::forward<TArgs>(mArgs)...));
c->entity = this;
std::unique_ptr<Component> uPtr{ c };
components.emplace_back(std::move(uPtr));

// Add component to comp array and vector
componentArray[getComponentTypeID<T>()] = c;
componentBitSet[getComponentTypeID<T>()] = true;

c->init();
return *c;
}

// Is this entity in the group?
bool hasGroup(Group eGroup) const noexcept
{
return groupBitset[eGroup];
}

void addGroup(Group group) noexcept;
// Remove entity from group
void delGroup(Group group) noexcept { groupBitset[group] = false; }

// Returns a refrence to type component
template<typename T> T& getComponent() const
{
auto ptr(componentArray[getComponentTypeID<T>()]);
return *static_cast<T*>(ptr);
}

};

class EntityManager
{
private:
std::vector<std::unique_ptr<Entity>> entities;

// Holds an array of pointers to all entitys in a specific group
std::array<std::vector<Entity*>, maxGroups> groupedEntities;

public:
void update()
{
for (auto& ent : entities) ent->update();
}
void draw()
{
for (auto& ent : entities) ent->draw();
}

// Add entity to group through manager, can also be don
// through the entity itself
void addToGroup(Entity * entity, Group group)
{
groupedEntities[group].emplace_back(entity);
}

// Returns a vector of pointers to all entitys in a group
std::vector<Entity*>& getEntitiesByGroup(Group group)
{
return groupedEntities[group];
}

// Only update components of type T // Does this work or not?
template<typename T>
void updateComponentOfType()
{
for (auto& ent : entities)
if (ent->hasComponent<T>())
ent->update();
}
// Only draw components of type T
template<typename T>
void drawComponentOfType()
{
for (auto& ent : entities)
if (ent->hasComponent<T>())
ent->draw();
}

// Remove all non active entities
void refresh()
{
entities.erase(std::remove_if(std::begin(entities), std::end(entities),
[](const std::unique_ptr<Entity> &mEntity)
{
return !mEntity->isActive();
}),
std::end(entities));
}

// Add an entity to the world
// Add it to the entity managers vector of entities
Entity& addEntity()
{
Entity* e = new Entity(*this);
std::unique_ptr<Entity> uPtr{ e };
entities.emplace_back(std::move(uPtr));
return *e;
}
};

*/