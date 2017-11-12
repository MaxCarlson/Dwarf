#pragma once
#include "include\libtcod.hpp"
#include "ECS\EntityManager.h"
#include "ECS\World.h"
#include "EntityFactory.h"



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

	// Holds all the entities active
	// as well as all the components and systems
	World world;

	// Used for producing all Entities simply
	EntityFactory factory;

	// Systems
	RenderSystem * renderSystem;
	CameraSystem * cameraSystem;

	// Holds entity ID of camera
	// so we can do a lookup if need be
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

