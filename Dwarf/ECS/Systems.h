#pragma once
#include "EntityManager.h"

#include <vector>


class SystemBase
{
public:

	//SystemBase(const Filter & filter);
	virtual ~SystemBase() = 0;


	virtual void initialize() {}
	virtual void update() {};

	// Returns all entitys of a system
	const std::vector<Entity>& getEntities() const {
		return s_entities;
	};

private:
	// The component filter // Bitset to & against components of entitys to test if they have correct components for system
	//Filter m_filter;

	std::vector<Entity> s_entities;
};

template<class RequireList, class ExcludeList = Excludes<>>
class System : public SystemBase
{

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