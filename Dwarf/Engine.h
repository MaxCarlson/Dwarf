#pragma once
#include "include\libtcod.hpp"
#include "Map\MapRender.h"
#include "ECS\EntityManager.h"
#include "ECS\World.h"
#include "EntityFactory.h"
#include <unordered_map>
#include <memory>

class TileFactory;
class RenderSystem;
class MovementSystem;
class MovementAiSystem;
class AiWorkSystem;
class MiningAi;
class BuildAi;
class MiningSystem;
class EntityPositionCache;
class DijkstraMapsHandler;
class EquipHandler;
class InputHandler;
class DesignationHandler;
class WorkOrders;
class StockpileSystem;
class HaulingSystem;
class GuiSystem;
class AiArchitecture;
class WoodcuttingAi;
class CalenderSystem;
class PlantSystem;

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

	// Systems ~ move these into a sepperate wrapper?
	// also probably don't need to be public if they stay here?
	GuiSystem * guiSystem;
	RenderSystem * renderSystem;
	MovementSystem * movementSystem;
	MovementAiSystem * movementAiSystem;	
	AiWorkSystem * aiWorkSystem;
	MiningAi * miningAi;
	EquipHandler * equipHandler;
	BuildAi * buildAi;
	WorkOrders * workOrders;
	StockpileSystem * stockpileSystem;
	HaulingSystem * haulingSystem;
	AiArchitecture * aiArchitecture;
	WoodcuttingAi * woodcuttingAi;

	// Systems that don't touch Entities
	CalenderSystem * calenderSystem;
	InputHandler * inputHandler;
	DesignationHandler * designationHandler;
	MiningSystem * miningSystem;
	DijkstraMapsHandler * dijkstraHandler;
	EntityPositionCache * entityPositionCache;
	PlantSystem * plantSystem;

	// Vector of dwarven Entities
	// for fast lookup anywhere in program
	// Possibly cache all Entities by their Coordinates in an unorederdmulti_map?????

	// These need to be pointers because their id's won't be up to date!!!!!!!!!!!!!!!!???????????????????
	// Get rid of these?
	std::vector<Entity> Dwarves;

	int screenWidth; // Git rid of these?
	int screenHeight;

	void newGame(int screenWidth, int screenHeight);
	void loadGame(std::string filePath);
	void saveGame(std::string filePath);

	void regComponents();
	void init();

	// Game loop
	void run();

	// Update systems
	// take inputs
	void update(double deltaTime, bool majorTick);


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

