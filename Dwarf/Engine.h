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

	int screenWidth;
	int screenHeight;
	TCOD_key_t lastKey;

	Engine(int screenWidth, int screenHeight);
	~Engine();

	void update();
	void render();
	void sendToBack(Actor * actor);

	int fovRadius; // 0 for infinite

private:
};

extern Engine engine;

