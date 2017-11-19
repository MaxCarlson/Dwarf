#include "MovementAiSystem.h"
#include "../Tile.h"

// Move these features to tileManager or keep here?
inline bool PathGraph::passable(Coordinates co) const
{
	return !(tileManager->getProperty<TileManager::WALL>(co) | tileManager->getProperty<TileManager::OBSTRUCTED>(co)) && tileManager->getProperty<TileManager::FLOOR>(co);
}

MovementAiSystem::MovementAiSystem(TileManager * tileManager) : tileManager(tileManager)
{
	pathGraph.tileManager = tileManager;
	pathGraph.width = tileManager->width;
	pathGraph.height = tileManager->height;
	pathGraph.depth = tileManager->depth;
}

MovementAiSystem::~MovementAiSystem()
{
	delete tileManager;
}

void MovementAiSystem::initMap(TileManager * tileMan)
{
	tileManager = tileMan;
	pathGraph.tileManager = tileManager;
	pathGraph.width = tileManager->width;
	pathGraph.height = tileManager->height;
	pathGraph.depth = tileManager->depth;
}

void MovementAiSystem::update()
{
	const auto& entities = getEntities();


	for (auto& e : entities)
	{


	}
}


