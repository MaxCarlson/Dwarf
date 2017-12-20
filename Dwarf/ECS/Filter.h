#pragma once

#include "TypeID.h"
#include "Component.h"
#include <type_traits>

template<class... Args>
struct TypeList {};

struct BaseRequires {};
struct BaseExcludes {};
struct BaseAny {};

struct Filter
{
public:

	Filter(ComponentTypeList requires, ComponentTypeList excludes, ComponentTypeList any)
		: requires(requires), excludes(excludes), any(any) {};

	// If the filter requires a component and it doesn't have it
	// return false
	// If and of the components on type list match the exclude list
	// return false
	// Else return true
	bool doesPassFilter(const ComponentTypeList& typeList) const
	{
		// If any of the entities componenents match the any type list
		// It automatically passes
		if ((any & typeList).any())
			return true;

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
	ComponentTypeList any;
};

template <class... Args>
static ComponentTypeList types(TypeList<Args...> typeList) { return ComponentTypeList(); }

template <class T, class... Args>
static ComponentTypeList types(TypeList<T, Args...> typeList)
{
	static_assert(std::is_base_of<Component, T>::value, "Invalid component");
	return ComponentTypeList().set(ComponentTypeId<T>()) | types(TypeList<Args...>());
}

// Make a Filter for System that Requires RequireList Componenets
// and Excludes ExcludeList Componenets
template<class RequireList, class ExcludeList, class AnyList>
Filter MakeFilter()
{
	static_assert(std::is_base_of<BaseRequires, RequireList>::value, "RequireList is not a requirement list");
	static_assert(std::is_base_of<BaseExcludes, ExcludeList>::value, "ExcludeList is not an excludes list");
	static_assert(std::is_base_of<BaseAny, AnyList>::value, "AnyList is not an any list");
	return Filter{ types(RequireList{}), types(ExcludeList{}), types(AnyList{}), };
}


template<class... Args>
struct Requires : TypeList<Args...>, BaseRequires {};

template<class... Args>
struct Excludes : TypeList<Args...>, BaseExcludes {};

template<class... Args>
struct Any : TypeList<Args...>, BaseAny {};