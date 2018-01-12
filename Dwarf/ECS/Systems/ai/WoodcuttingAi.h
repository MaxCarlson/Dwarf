#pragma once
#include "../../Systems.h"
#include "../../Components/Tags/LumberjacTag.h"


class WoodcuttingAi : public System<Requires<LumberjacTag>>
{
public:
	void init();
	void update(const double duration);

private:
	void doWork(Entity& e, const double & duration);
};

