#pragma once

#include "../../Systems.h"

class JobComponent;
class PositionComponent;

class AiWorkSystem : public System<Requires<JobComponent, PositionComponent>> // Replace this component
{
public:
	AiWorkSystem();
	~AiWorkSystem();

	void update();
};

