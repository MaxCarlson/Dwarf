#pragma once
#include "ECS\Systems.h"
#include "ECS\Components\ai\View.h"
#include "ECS\Components\PositionComponent.h"

class ViewSystem : public System<Requires<View, PositionComponent>>
{
public:
	void init();
	void update(const double duration);

private:
	bool first = true;
	std::unordered_set<size_t> dirtyEnts;
};