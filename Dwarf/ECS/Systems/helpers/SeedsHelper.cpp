#include "SeedsHelper.h"


SeedsHelper seedsHelper;

void SeedsHelper::forAllUnclaimedSeeds(std::function<void(Entity)> func)
{
	for (const auto& e : getEntities())
		func(e);
}