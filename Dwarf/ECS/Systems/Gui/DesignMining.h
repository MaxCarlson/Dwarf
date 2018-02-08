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

	bool confirm = false;
	int miningType = 0;

	void resetGui();
	void loopThroughPossibleMining(int type, Coordinates sml, Coordinates lrg, std::function<void(bool, int, int)> onPossible);

	void drawPossibleMining();
	void designate();
	void drawErasure();
};

