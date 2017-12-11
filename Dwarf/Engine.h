#pragma once
#include "include\libtcod.hpp"
#include "ECS\EntityManager.h"
#include "ECS\World.h"
#include "EntityFactory.h"
#include "Gui.h"
#include "Input.h"
#include <unordered_map>

class Map;
class RenderSystem;
class MovementSystem;
class MovementAiSystem;
class AiWorkSystem;
class MiningAi;
class MiningSystem;
class EntityPositionCache;
class DijkstraMapsHandler;
class EquipHandler;


class Engine
{
public:
	Engine() = default;
	~Engine();
	
	// Holds all the entities active
	// as well as all the components and systems
	World world;

	// Used for producing all Entities simply
	EntityFactory factory;

	// Systems
	RenderSystem * renderSystem;
	MovementSystem * movementSystem;
	MovementAiSystem * movementAiSystem;	
	AiWorkSystem * aiWorkSystem;
	MiningAi * miningAi;
	EquipHandler * equipHandler;

	// Systems that don't touch Entities
	MiningSystem * miningSystem;
	DijkstraMapsHandler * dijkstraHandler;
	EntityPositionCache * entityPositionCache;

	// Vector of dwarven Entities
	// for fast lookup anywhere in program
	// Possibly cache all Entities by their Coordinates in an unorederdmulti_map?????

	// These need to be pointers because their id's won't be up to date!!!!!!!!!!!!!!!!???????????????????
	std::vector<Entity> Dwarves;

	// Holds local map, 
	// Local map also holds the map renderer
	Map * map;


	// Should these be made into systems?
	Gui gui;
	Input input;

	int screenWidth;
	int screenHeight;

	void init(std::string mapPath, int screenWidth, int screenHeight);
	void loadMap(std::string filePath);

	// Game loop
	void run();

	// Update systems
	// take inputs
	void update(double deltaTime);


	enum GameStates
	{
		PLAY,
		PAUSED,
		PLAY_TO_ESC,
		ESC_MENU,
		ESC_TO_PLAY,

		TO_MAIN_MENU
	} 
	current_game_state = PLAY;

private:
	// Render local map 
	// as well as Entities on it
	void render();
};

extern std::unique_ptr<Engine> engine;

