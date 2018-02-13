#include "SeedsHelper.h"


SeedsHelper seedsHelper;

void SeedsHelper::forAllSeeds(std::function<void(Entity)> func)
{
	for (const auto& e : getEntities())
		func(e);
}