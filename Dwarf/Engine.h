#pragma once

#include "include\libtcod.hpp"


class Actor;
class Map;

class Engine
{
public:
	TCODList<Actor *> actors;
	Actor * player;
	Map * map;

	Engine();
	~Engine();

	void update();
	void render();

	int fovRadius; // 0 for infinite

private:
	bool computeFov;
};

extern Engine engine;

