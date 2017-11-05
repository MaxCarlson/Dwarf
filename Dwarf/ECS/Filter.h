#pragma once

#include "TypeID.h"
#include "Component.h"


template<class... Args>
struct TypeList {};

struct BaseRequires {};
struct BaseExcludes {};

struct Filter
{
public:

	Filter(ComponentTypeList requires, ComponentTypeList excludes)
		: requires(requires), excludes(excludes) {};

	// If the filter requires a component and it doesn't have it
	// return false
	// If and of the components on type list match the exclude list
	// return false
	// Else return true
	bool doesPassFilter(const ComponentTypeList& typeList) const
	{
		for (std::size_t i = 0; i < requires.size(); ++i)		
			if (requires[i] && !typeList[i])
				return false;
		
		if ((excludes & typeList).any())
			return false;

		return true;
	}

private:
	// Bit sets of the required and excuded Component
	// type lists set by Filter init
	ComponentTypeList requires;
	ComponentTypeList excludes;
};

template<class... Args>
static ComponentTypeList types(TypeList<Args...> typeList) { return ComponentTypeList(); }

template<class T, class... Args>
static ComponentTypeList types(TypeList<T, Args...> typeList)
{
	static_assert(std::is_base_of<Component, T>::value, "Not a component!");
	return ComponentTypeList().set(ComponentTypeId<T>() | types(TypeList < Args...>)());
}

// Make a Filter for System that Requires RequireList Componenets
// and Excludes ExcludeList Componenets
template<class RequireList, class ExcludeList>
Filter MakeFilter()
{
	static_assert(std::is_base_of<BaseRequires, RequireList>::value, "RequireList is not a requirement list");
	static_assert(std::is_base_of<BaseExcludes, ExcludeList>::value, "ExcludeList is not an excludes list");
	return Filter{ types(RequireList{}, types(ExcludeList{}) };
}

template<class... Args>
struct Requires : TypeList<Args...>, BaseRequires {};

template<class... Args>
struct Excludes : TypeList<Args...>, BaseExcludes {};