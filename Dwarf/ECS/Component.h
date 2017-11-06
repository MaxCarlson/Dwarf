#pragma once

#include "TypeID.h"

#include <vector>
#include <bitset>

// Max components entity can hold
constexpr std::size_t MAX_COMPONENTS = 64;

class Component
{
public:
	virtual ~Component() {};
};

// Returns either a new unique Id for a component
// if it's the first time that component has been added
// or the existing Id of component type T.
template<class T>
TypeId ComponentTypeId()
{
	return ClassTypeId<Component>::getTypeId<T>();
}

using ComponentArray    = std::vector<Component*>;
// A BitSet of which components this entity currently has
using ComponentTypeList = std::bitset<MAX_COMPONENTS>;