#pragma once

#include "TypeID.h"

#include <vector>
#include <bitset>
#include "../cereal/types/string.hpp"
#include "../cereal/types/polymorphic.hpp"
#include "../cereal/types/base_class.hpp"
#include "../cereal/archives/binary.hpp"

// Max components entity can hold
// This can be changed to whatever, best keep it power of two and small
constexpr std::size_t MAX_COMPONENTS = 64;

class Component
{
public:
	virtual ~Component() {};

	template <class Archive>
	void serialize(Archive & ar)
	{
	}
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