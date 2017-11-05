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

	//std::atomic<TypeId> nextTypeId();

private:
	static std::atomic<TypeId> nextTypeId;
};

template <typename BaseClass>
static std::atomic<TypeId> ClassTypeId<BaseClass>::nextTypeId{ 0 };

/*
template<typename BaseClass>
inline std::atomic<TypeId> ClassTypeId<BaseClass>::nextTypeId()
{
	return std::atomic<TypeId>();
}
*/
