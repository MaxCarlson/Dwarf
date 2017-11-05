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
	template<typename BaseClass>
	static TypeId getTypeId()
	{
		static const TypeId id = nextTypeId++;
		return id;
	}

private:
	static std::atomic<TypeId> nextTypeId;
};

template <typename BaseClass>
std::atomic<TypeId> ClassTypeId<BaseClass>::nextTypeId{ 0 };
