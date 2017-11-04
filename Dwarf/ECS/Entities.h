#pragma once

#include "Component.h"

#include <cstdint>

typedef std::uint64_t Id;

class Entity
{
public:

	Id id;

	Entity();

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
};

