#pragma once
#include "EntityManager.h"

#include <vector>

template<class C> class ComponentTable
{
	std::vector<C> components;
public:
	C& findComponent(ComponentID id)
};

class SystemBase
{
public:
	//SystemBase(const Filter & filter);
	virtual ~SystemBase() = 0;


	virtual void initialize() {}
	virtual void update() {};

	// Returns all entitys of a system
	const std::vector<Entity>& getEntities() const {
		return m_entities;
	};

private:
	// The component filter // Bitset to & against components of entitys to test if they have correct components for system
	//Filter m_filter;

	std::vector<Entity> m_entities;
};