#pragma once

#pragma once

#include "../../Systems.h"

#include "../../Components/Tags/MiningTag.h"

class TileManager;
class PositionComponent;

class MiningAi : public System<Requires<MiningTag, PositionComponent>>
{
public:
	MiningAi() = default;

	void init();

	void update(double deltaT);

private:
	void updateMiner(const Entity& e);
};