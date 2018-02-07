#pragma once
#include "ECS\Systems.h"
#include "Coordinates.h"

class DesignMining : public System<Requires<>>
{
public:
	void init();
	void update(const double duration);

private:
	Coordinates click = EMPTY_COORDINATES;

	enum MiningTypes
	{
		MINING,
		CHANNELING,
		RAMPING,
		UP_STAIRS,
		DOWN_STAIRS,
		UP_DOWN_STAIRS,
		ERASE
	};

	bool confirm = false;
	int miningType = 0;

	void drawPossibleMining();
	void designate();
	void drawErasure();
};

