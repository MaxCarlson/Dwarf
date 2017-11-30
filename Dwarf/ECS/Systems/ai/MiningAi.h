#pragma once

#pragma once

#include "../../Systems.h"

#include "../../Components/Tags/MiningTag.h"

class TileManager;

class MiningAi : public System<Requires<MiningTag>>
{
public:
	MiningAi() = default;
	MiningAi(TileManager * tileManager);

	void init();

	void update(double deltaT);

private:
	TileManager * tileManager;

	void updateMiner(Entity e);
};