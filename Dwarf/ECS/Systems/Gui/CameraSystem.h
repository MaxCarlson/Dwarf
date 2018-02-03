#pragma once
#include "ECS\Systems.h"

class CameraSystem : public System<Requires<>>
{
public:
	void init();
	void update(const double duration);
};

