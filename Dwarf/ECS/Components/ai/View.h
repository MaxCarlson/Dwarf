#pragma once
#include "ECS\Component.h"

struct View : public Component
{
	View() = default;
	View(int radius) : radius(radius) {}

	// How large is this entities sight radius
	int radius = 1;

	// Entities currently in view
	std::unordered_set<size_t> inView;

	// Cached idx's that are currently visible
	std::vector<int> visibleCache;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(radius, inView, visibleCache);
	}
};

CEREAL_REGISTER_TYPE(View);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, View);