#pragma once
#include "include\libtcod.hpp"
#include "ECS\EntityManager.h"
#include "ECS\World.h"


class Gui;
class Actor;
class Map;
class CameraSystem;
class RenderSystem;

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

	//EntityManager entityManager;
	//Entity * camera; // Change this to non pointer?

	World world;

	RenderSystem * renderSystem;
	CameraSystem * cameraSystem;

	Entity camera;


	TCOD_key_t lastKey;
	TCOD_mouse_t mouse;
	TCODList<Actor *> actors;
	Actor * player;
	Map * map;

	int screenWidth;
	int screenHeight;

	Gui *gui;

	Engine(int screenWidth, int screenHeight);
	~Engine();

	void update();
	void render();
	void sendToBack(Actor * actor);

	int fovRadius; // 0 for infinite

private:
};

extern Engine engine;

