#pragma once
#include "ECS/Systems.h"


class RegionHelper : public System<Requires<>>
{
public:
	void init();
	void update(const double duration);

	void performMining(Entity e, const int idx, const int type);

private:

};

extern std::unique_ptr<RegionHelper> regionHelper;