#include "MovementAiSystem.h"
#include "../Tile.h"
#include <queue>
#include <functional>

// Wrapper for the priority queue
// class so we can use pair's easily
template<typename T, typename priorityT>
struct PriorityQueue
{
	typedef std::pair<priorityT, T> Element;
	std::priority_queue<Element, std::vector<Element>, std::greater<Element>> elements;

	bool empty() const { return elements.empty(); }

	inline void put(T item, priorityT priority)
	{
		elements.emplace(priority, item);
	}

	inline T get()
	{
		T bestItem = elements.top().second;
		elements.pop();
		return bestItem;
	}
};

// Order of Coordinates in DIRS array below
// enum Directions N, NW, NE, S, SW, SE, E, W, UP, DOWN
std::array<Coordinates, 10> PathGraph::DIRS{ Coordinates{ 0, -1, 0 },{ -1, -1, 0 },{ 1, -1, 0 },{ 0, 1, 0 },{ -1, 1, 0 },{ 1, 1, 0 },{ 1, 0, 0 },{ -1, 0, 0 },{ 0, 0, 1 },{ 0, 0, -1 } };

// Introduce verticality eventually!!
inline double heuristic(Coordinates co, Coordinates co1)
{
	return std::abs(co.x - co1.x) + std::abs(co.y - co1.y);
}

PathGraph::~PathGraph()
{
	delete tileManager;
}

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

// Test doubles vs doubles in this and helper structs!!
void MovementAiSystem::aStar(Coordinates start, Coordinates end)
{
	PriorityQueue<Coordinates, double> frontier;
	frontier.put(start, 0);

	std::unordered_map<Coordinates, double>  costSoFar;
	std::unordered_map<Coordinates, Coordinates> path;

	path[start]  = start;
	costSoFar[start] = 0;

	while (!frontier.empty())
	{
		auto current = frontier.get();

		// Add path to entity queue first
		if (current == end)
			break;

		// Loop through all neighbors of currrent tile
		for (auto& next : pathGraph.neighbors(current))
		{
			double newCost = costSoFar[current] + pathGraph.cost(current, next);

			if (!costSoFar.count(next) || newCost < costSoFar[next])
			{
				costSoFar[next] = newCost;
				double priority = newCost + heuristic(next, end);

				frontier.put(next, priority);
				path[next] = current;
			}
		}
	}

}

void MovementAiSystem::pathToEntity(std::unordered_map<Coordinates, Coordinates> path)
{
}


