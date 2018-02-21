#pragma once
#include "ECS\Systems.h"
#include "ECS\Components\Tags\JoyTag.h"

class JoySystem : public System<Requires<JoyTag>>
{
public:
	void init();
	void update(const double duration);
};

