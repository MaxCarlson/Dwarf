#pragma once
#include "../../Systems.h"
#include "../../Components/Item.h"

class ItemHelper : public System<Requires<Item>>
{
public:
	ItemHelper() = default;

	template<typename... Args, typename T>
	auto forEachItem(T t, Args&&... args);
};

template<typename... Args, typename T>
inline auto ItemHelper::forEachItem(T t, Args && ...args)
{
	auto f = forEachItem(std::forward<Args>(args)...);

	return f;
}

/*
template<typename F, typename... G>
auto forEachItem(F&& func, G&&... getters) {
return std::forward<F>(func)(std::forward<G>(getters)(0)...);
}
*/
