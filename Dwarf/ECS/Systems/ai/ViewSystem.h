#pragma once
#include "ECS\Systems.h"
#include "ECS\Components\ai\View.h"

class ViewSystem : public System<Requires<View>>
{
public:
	void init();
	void update(const double duration);
};