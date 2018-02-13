#pragma once
#include "../../Components/Seed.h"
#include "../../Components/Claimed.h"

class SeedsHelper : public System<Requires<Seed>>
{
public:
	void forAllSeeds(std::function<void(Entity)> func);
};

extern SeedsHelper seedsHelper;