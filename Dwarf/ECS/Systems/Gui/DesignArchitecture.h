#pragma once
#include "ECS\Systems.h"
#include "Coordinates.h"

class DesignArchitecture : public System<Requires<>>
{
public:
	void init();
	void update(const double duration);

private:
	enum Mode
	{
		DRAW,
		ERASE_ALL,
		ERASE_SELECT,
	};

	int mode = DRAW;

	Coordinates click1 = EMPTY_COORDINATES;
	int selected = 0;
	void drawDesignation();
};

