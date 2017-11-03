#pragma once


#include <memory>
#include <bitset>
#include <algorithm>
#include <array>


class Component;
class Entity;

using ComponentID = std::size_t;

// Component ID's by order of initialization
inline ComponentID getComponentTypeID()
{
	static ComponentID lastID = 0;
	return ++lastID;
}
template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	static ComponentID typeID = getComponentTypeID();
	return typeID;
}

// Max components entity can hold
constexpr std::size_t maxComponents = 32;

// Used for matching entities to component sets
using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray  = std::array<Component*, maxComponents>;


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
public:
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
		return ComponentBitSet[getComponentTypeID<T>()]; // Check here if bugs!!
	}

	
	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		// Add component to component vector
		// passing any arguments to constructers we may need
		T* c(new T(std::forward<TArgss>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		// Add component to comp 
		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

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

public:
	void update()
	{
		for (auto& ent : entities) ent->update();
	}
	void draw() 
	{
		for (auto& ent : entities) ent->draw();
	}
	// Only update components of type T // Does this work or not?
	template<typename T>
	void updateComponentOfType()
	{	
		for (auto& ent : entities->hasComponent<T>())  ent->update();
	}
	// Only draw components of type T
	template<typename T>
	void drawComponentOfType()
	{
		for (auto& ent : entities->hasComponent<T>()) ent->draw();
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
		Entity* e = new Entity();
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
}; extern EntityManager entityManager;







/*
class EntityManager : GameObject
{
public:
	EntityManager();
	Id generateNewId();
	Entity * createEntity();
	void addComponent(Component * comp, Entity * ent);
	Component * getComponentOfClass();
	void removeEntity(Entity *);

	Id lowestVacantId = 1;
	std::vector<Entity *> entities;
	// Holds components mapped by their enum CompID's
	std::map<CompID, std::map<Id, Component>*> componentsByClass; 
};

*/