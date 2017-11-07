#pragma once
#include "include\libtcod.hpp"
#include "ECS\EntityManager.h"
#include "ECS\World.h"



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

	// Systems
	RenderSystem * renderSystem;
	CameraSystem * cameraSystem;

	Entity camera;


	TCOD_key_t lastKey;
	TCOD_mouse_t mouse;

	Map * map;

	int screenWidth;
	int screenHeight;


	Engine(int screenWidth, int screenHeight);
	~Engine();

	void update();
	void render();

	int fovRadius; // 0 for infinite

private:
};

extern Engine engine;

