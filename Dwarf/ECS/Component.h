#pragma once

#include "TypeID.h"

#include <vector>

class Component
{
public:
	virtual ~Component();
};

// Returns either a new unique Id for a component
// if it's the first time that component has been added
// or the existing Id of component type T.
template<class T>
TypeId ComponentTypeId()
{
	return ClassTypeId<Component>::getTypeId<T>();
}

using ComponentArray = std::vector<Component*>;