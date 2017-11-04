#pragma once

#include <atomic>



typedef size_t TypeId;


// Template class used for generating and finding
// the unique TypeId of the component or system.
template<typename BaseClass>
class ClassTypeId
{
public:
	template<typename TBase>
	static TypeId getTypeId()
	{
		static const TypeId id = nextTypeId++;
		return id;
	}

private:
	static std::atomic<TypeId> nextTypeId;
};

template <typename TBase>
static std::atomic<TypeId> ClassTypeId<TBase>::nextTypeId{ 0 };
