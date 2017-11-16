#pragma once
#include "include\libtcod.hpp"
#include "ECS\EntityManager.h"
#include "ECS\World.h"
#include "EntityFactory.h"
#include "Gui.h"
#include "Input.h"


class Map;
class CameraSystem;
class RenderSystem;

class Engine
{
public:

	// Holds all the entities active
	// as well as all the components and systems
	World world;

	// Used for producing all Entities simply
	EntityFactory factory;

	// Systems
	RenderSystem * renderSystem;

	// Holds entity ID of camera
	// so we can do a lookup if need be
	//Entity camera;

	// Holds local map, 
	// Local map also holds the map renderer
	Map * map;

	Gui gui;

	Input input;

	int screenWidth;
	int screenHeight;


	Engine(int screenWidth, int screenHeight);
	~Engine();

	// Game loop
	void run();

	// Update systems
	// take inputs
	void update();

	// Render local map 
	// as well as Entities on it
	void render();

private:
};

extern Engine engine;

