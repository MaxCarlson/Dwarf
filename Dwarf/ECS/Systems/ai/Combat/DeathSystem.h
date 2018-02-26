#pragma once
#include "ECS\Component.h"

class DeathSystem : public System<Requires<>> // Should require health component?
{
public:
	void init();
	void update(const double duration);
};