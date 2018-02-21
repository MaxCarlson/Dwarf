#pragma once
#include "ECS\World.h"

extern World world;

// Loop through entities that have Required<Components...>,
// who don't have Excluded<Components...>, and any that have Any<Components...>
// &&
// You'll want to instatiate the system along with your other systems
// at world startup with a short llamabda:
// eachWith<Requires<cr ...cn>, Excludes<ce ...cn>, Any<ca ...cn>>([](auto e){}); 
// Excludes and Any are optional
// &&
// Reset is used when you would like to clean world of all systems
// and possibly restart game
template<class Requires, class Excludes = Excludes<>, class Any = Any<>>
void eachWith(std::function<void(const Entity&)>&& func, bool reset = false, bool first = false)
{
	static SystemBase* sys;

	// Instantiate system if first time function with params is called
	if (first)
	{
		first = false;
		sys = new System<Requires, Excludes, Any>;
		world.addVariadicSystem(*sys);
	}

	// If qutting game flag for re-creation of system if game starts again
	// Memory is managed in world for system deletion
	else if (reset)
	{
		first = true;
		sys = nullptr;
		return;
	}

	for (const auto& e : sys->getEntities())
		func(e);
}