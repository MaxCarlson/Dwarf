#pragma once
#include "ECS\Systems.h"

class PatrolSystem : public System<Requires<>>
{
public:
	void init();
	void update(const double duration);
private:
};