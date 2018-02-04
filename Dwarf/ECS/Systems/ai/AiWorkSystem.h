#pragma once

#include "../../Systems.h"
#include "../../Components/Sentients/AiWorkComponent.h"

class PositionComponent;

class AiWorkSystem : public System<Requires<AiWorkComponent, PositionComponent>> // Replace this component
{
public:
	AiWorkSystem() = default;


	void update(const double duration);
};

