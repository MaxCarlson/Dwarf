#pragma once

#include "Component.h"

#include <cstdint>

typedef std::uint64_t Id;

class Entity
{
public:

	Entity();

	inline const Id getId() const;

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

	// operators here

private:

	void addComponent(Component * component, TypeId componentTypeId);
	void removeComponent(TypeId componentTypeId);
	Component & getComponent(TypeId componentTypeId) const;
	bool hasComponent(TypeId componentTypeId) const;

	Id eId;
};

template<typename T, typename ...Args>
inline T & Entity::addComponent(Args && ...args)
{
	// TODO: insert return statement here
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
	return hasComponent(getComponent(ComponentTypeId<T>());
}


