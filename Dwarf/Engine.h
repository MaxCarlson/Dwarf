#pragma once

#include "include\libtcod.hpp"


class Actor;
class Map;

class Engine
{
public:
	enum GameStatus {
		STARTUP,
		IDLE,
		NEW_TURN,
		VICTORY,
		DEFEAT
	} gameStatus;
	TCODList<Actor *> actors;
	Actor * player;
	Map * map;

	Engine();
	~Engine();

	void update();
	void render();

	int fovRadius; // 0 for infinite

private:
};

extern Engine engine;

