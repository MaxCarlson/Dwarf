#pragma once
#include "ECS\Systems.h"
#include "ECS\Components\Sentients\AiWorkComponent.h"

class JobPreferencesUi : public System<Requires<AiWorkComponent>>
{
public:
	void init();
	void update(const double duration);
};

