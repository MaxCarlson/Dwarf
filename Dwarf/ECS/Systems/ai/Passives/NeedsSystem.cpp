#include "stdafx.h"
#include "NeedsSystem.h"

void NeedsSystem::init()
{
}

inline void processSleep(Need &sleep) // Add in negative thoughts!
{
	if (sleep.lvl > SleepThreshold::SLEEPY)
	{

	}

	else if (sleep.lvl > SleepThreshold::TIRED)
	{

	}

	else if (sleep.lvl > SleepThreshold::VERY_TIRED)
	{

	}

	else // Exhausted
	{

	}
}

inline void decreaseNeed(Need &need, const double& duration)
{
	need.lvl -= (duration * need.declineRate) / 1000.0;
}

void NeedsSystem::update(const double duration)
{
	for (const auto& e : getEntities())
	{
		auto& needs = e.getComponent<Needs>();

		int i = 0;
		for (auto& n : needs.needs)
		{
			decreaseNeed(n, duration);

			switch (static_cast<NeedIdx>(i))
			{
			case NeedIdx::THRIST:
				break;

			case NeedIdx::HUNGER:
				break;

			case NeedIdx::SLEEP:
				processSleep(n);
				break;
			}

			++i;
		}
	}
}
