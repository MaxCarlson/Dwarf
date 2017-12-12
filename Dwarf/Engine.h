#pragma once
#include "include\libtcod.hpp"
#include "Map\MapRender.h"
#include "ECS\EntityManager.h"
#include "ECS\World.h"
#include "EntityFactory.h"
#include "Gui.h"
#include "Input.h"
#include <unordered_map>
#include <memory>

class Map;
class TileFactory;
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
	EntityFactory factory; // Replace this with serialized read in data!

	// Performs the rendering of the map
	// and associated built with map generation tiles
	// Should these be moved to Entities, and an array setup for which to render depending on z level?
	std::unique_ptr<MapRender> mapRenderer;
	
	// Replace this with serialized read in data!
	TileFactory * tileFactory;

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
	std::unique_ptr<Map> map;


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
		SAVE_GAME,

		TO_MAIN_MENU
	} 
	current_game_state = PLAY;

private:
	// Render local map 
	// as well as Entities on it
	void render();
};

extern std::unique_ptr<Engine> engine;

