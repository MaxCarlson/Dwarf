#pragma once
#include "include\libtcod.hpp"
#include "ECS\EntityManager.h"
#include "ECS\World.h"
#include "EntityFactory.h"
#include "Gui.h"
#include "Input.h"
#include <unordered_map>

class Map;
class CameraSystem;
class RenderSystem;
class MovementSystem;
class MovementAiSystem;
class JobsSystem;
class MiningSystem;
class MiningAiSystem;

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
	MovementSystem * movementSystem;
	MovementAiSystem * movementAiSystem;
	JobsSystem * jobsSystem;
	MiningAiSystem * miningAiSystem;

	// Systems that don't touch Entities
	MiningSystem * miningSystem;

	// Vector of dwarven Entities
	// for fast lookup anywhere in program
	// Possibly cache all Entities by their Coordinates in an unorederdmulti_map?????
	std::vector<Entity> Dwarves;

	// Cache of all alive Entities indexed by their coordinates
	// Worth using? Will have to test
	//std::unordered_multimap<Coordinates, Entity, CoordinateHash, CoordinateHashEqual> EntityPosCache;

	// Holds local map, 
	// Local map also holds the map renderer
	Map * map;

	Gui gui;

	Input input;

	int screenWidth;
	int screenHeight;

	
	Engine();
	~Engine();

	void init(int screenWidth, int screenHeight);

	// Game loop
	void run();

	// Update systems
	// take inputs
	void update(double deltaTime);

	// Render local map 
	// as well as Entities on it
	void render();

private:
};

extern Engine engine;

