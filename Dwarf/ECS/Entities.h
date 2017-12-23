#pragma once

#include "Component.h"


#include <cstdint>

class World;

class Entity
{
public:

	struct Id
	{
		Id() : index(0), counter(0) {};

		Id(std::uint64_t index, std::uint32_t counter)
			: index(index), counter(counter) {};

		void clear() { index = counter = 0; }

		// Id comparison overload
		//bool operator==(const Id& id);

		//bool operator==(const Entity e)
		//{
		//	return(e.eId.index == index && e.eId.counter == counter);
		//}

		std::uint64_t index;
		std::uint32_t counter;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(index, counter);
		}
	};

	Entity();

	Entity(World & entityWorld, Id id);

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(eId);
	}

	// Used to reset world when loading ecs
	// from serilization
	void setWorld(World & eworld)
	{
		world = &eworld;
	}


	Entity(const Entity &)			  = default;
	Entity(Entity &&)			      = default;
	Entity& operator=(const Entity &) = default;
	Entity& operator=(Entity &&)      = default;

	// Checks whether entity is valid
	bool isValid() const;

	// Returns a refrence to the world this 
	// Entity belongs to
	World& getWorld() const;

	// Returns Entities unique ID
	const Id getId() const;

	// Returns true if entity has been activated
	bool isActivated() const;

	// Activate Entity
	void activate();

	// deactivate Entity
	void deactivate();

	// Kill Entity
	void kill();

	// Add component of type T
	// args = argument/s of constructor for components
	template <typename T, typename... Args>
	T& addComponent(Args&&... args);

	// Removes type T component
	template <typename T>
	void removeComponent();

	// Removes all components
	void removeAllComponent();

	// Returns a refrence to component
	// if entity holds T component
	template<typename T>
	T& getComponent() const;

	// Returns true if entity has 
	// Component of type T
	template<typename T>
	bool hasComponent() const;

	// Returns a vector of *'s to all components
	// this entity has
	ComponentArray getAllComponents() const;

	ComponentTypeList getComponentTypeList() const;

	// Comparison operators
	bool operator==(const Entity& entity) const;
	bool operator!=(const Entity& entity) const { return !operator==(entity); }

private:

	void addComponent(Component * component, TypeId componentTypeId);
	void removeComponent(TypeId componentTypeId);
	Component & getComponent(TypeId componentTypeId) const;
	bool hasComponent(TypeId componentTypeId) const;

	// Id of Entity
	// Used for indexing almost everywhere
	Id eId;

	// Pointer to world this entity belongs to
	World * world;
};

template<typename T, typename ...Args>
inline T & Entity::addComponent(Args && ...args)
{
	auto component = new T{ std::forward<Args>(args)... };
	addComponent(component, ComponentTypeId<T>());
	return *component;
}

template<typename T>
inline void Entity::removeComponent()
{
	removeComponent(ComponentTypeId<T>());
}

template<typename T>
T& Entity::getComponent() const
{
	return static_cast<T&>(getComponent(ComponentTypeId<T>()));
}

template<typename T>
bool Entity::hasComponent() const
{
	return hasComponent(ComponentTypeId<T>());
}


