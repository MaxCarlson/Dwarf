#include "stdafx.h"
#include "NeedsSystem.h"

void NeedsSystem::init()
{
}

inline void processThirst(Need &thirst)
{
	if (thirst.lvl > ThirstThreshold::THIRSTY)
	{
		thirst.lvl -= NeedDeclines::thirst;
	}
	else if (thirst.lvl > ThirstThreshold::PARCHED)
	{
		thirst.lvl -= NeedDeclines::thirst / 2;
	}

	else if (thirst.lvl > ThirstThreshold::VERY_THRISTY)
	{
		thirst.lvl -= NeedDeclines::thirst / 4;
	}

	else // Dehydrated
	{
		thirst.lvl -= NeedDeclines::thirst / 8;
	}
}

inline void processHunger(Need &hunger)
{
	if (hunger.lvl > HungerThreshold::HUNGRY)
	{
		hunger.lvl -= NeedDeclines::hunger;
	}

	else if (hunger.lvl > HungerThreshold::FAMISHED)
	{
		hunger.lvl -= NeedDeclines::hunger / 2;
	}

	else if (hunger.lvl > HungerThreshold::URGENTLY)
	{
		hunger.lvl -= NeedDeclines::hunger / 4;
	}

	else // Starving
	{
		hunger.lvl -= NeedDeclines::hunger / 8;
	}

}

inline void processSleep(Need &sleep) // Add in negative thoughts!
{
	if (sleep.lvl > SleepThreshold::SLEEPY)
	{
		sleep.lvl -= NeedDeclines::sleep;
	}

	else if (sleep.lvl > SleepThreshold::TIRED)
	{
		sleep.lvl -= NeedDeclines::sleep / 2;
	}

	else if (sleep.lvl > SleepThreshold::VERY_TIRED)
	{
		sleep.lvl -= NeedDeclines::sleep / 4;
	}

	else // Exhausted
	{
		sleep.lvl -= NeedDeclines::sleep / 8;
	}

}

inline void processComfort(Need &comfort)
{
	comfort.lvl -= NeedDeclines::comfort;
}

inline void processJoy(Need &joy) // TODO: Possibly condense these into one function using a vector of stored decline values and a vector of thresholds?
{
	joy.lvl -= NeedDeclines::joy;
}

void NeedsSystem::update(const double duration)
{
	for (const auto& e : getEntities())
	{
		auto& needs = e.getComponent<Needs>();

		int i = 0;
		for (auto& n : needs.needs)
		{
			switch (static_cast<NeedIdx>(i))
			{
			case NeedIdx::THIRST:
				processThirst(n);
				break;

			case NeedIdx::HUNGER:
				processHunger(n);
				break;

			case NeedIdx::SLEEP:
				processSleep(n);
				break;

			case NeedIdx::COMFORT:
				processComfort(n);
				break;

			case NeedIdx::JOY:
				processJoy(n);
				break;
			}

			if (n.lvl < 0.0)
				n.lvl = 0.0;
			++i;
		}
	}
}
