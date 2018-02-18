#pragma once

#include <cstddef>
#include <atomic>



typedef std::size_t TypeId;


// Template class used for generating and finding
// the unique TypeId of the component or system.
template<typename BaseClass>
class ClassTypeId
{
public:

	// Used for written out classes
	template<typename BaseClass>
	static TypeId getTypeId()
	{
		static const TypeId id = nextTypeId++;
		return id;
	}

	// Used for Variadic each template systems
	// Must never instantiate a variadic each out of order
	// Will cause entity system beloning to be possibly messed up
	static TypeId autoTypeId()
	{
		static const TypeId firstVariadicTypeId = (startOfVariadics = nextTypeId);
		return nextTypeId++;
	}

	static void resetTypeIdForVariadics()
	{
		nextTypeId = startOfVariadics;
	}

private:
	// Index of first variadic sytem registered.
	// This is here so we can fall back to the correct typeId index
	// when restarting game while still inside the process
	static TypeId startOfVariadics;

	static std::atomic<TypeId> nextTypeId;
};

template <typename BaseClass>
std::atomic<TypeId> ClassTypeId<BaseClass>::nextTypeId{ 0 };

template <typename BaseClass>
TypeId ClassTypeId<BaseClass>::startOfVariadics{ 0 };
