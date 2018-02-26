#pragma once

#include "../../Systems.h"
#include "../../Components/Sentients/AiWorkComponent.h"
#include "ECS\Components\Fighting\Drafted.h"

class PositionComponent;

class AiWorkSystem : public System<Requires<AiWorkComponent, PositionComponent>, Excludes<Drafted>> 
{
public:
	AiWorkSystem() = default;


	void update(const double duration);
};

