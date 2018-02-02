#pragma once

#include "../../Systems.h"
#include "../../Components/JobComponent.h"
//class JobComponent;
class PositionComponent;

class AiWorkSystem : public System<Requires<JobComponent, PositionComponent>> // Replace this component
{
public:
	AiWorkSystem() = default;


	void update(const double duration);
};

