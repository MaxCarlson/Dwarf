#include "stdafx.h"
#include "HappinessSystem.h"

/*
void HappinessSystem::init()
{
}

inline void decreaseNeeds(const double& duration, Needs &needs)
{
	for (auto& n : needs.needs)
	{
		n.lvl -= (duration * n.declineRate) / 1000.0;
	}
}

inline void adjustHappiness(Happiness &happiness, Needs &needs)
{
	
	happiness.total = 0.0;
	for (const auto& n : needs.needs)
		happiness.total += n.bonus / (MAX_NEED_FULFILMENT / n.lvl);
	
}

void HappinessSystem::update(const double duration)
{
	for (const auto& e : getEntities())
	{
		auto& needs = e.getComponent<Needs>();
		auto& hap = e.getComponent<Happiness>();

		decreaseNeeds(duration, needs);
		adjustHappiness(hap, needs);
	}
}
*/