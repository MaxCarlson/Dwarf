#include "MovementAiSystem.h"
#include "../Tile.h"
#include "../Components/PositionComponent.h"
#include <queue>
#include <functional>
#include <utility>




// Wrapper for the priority queue
// class so we can use pair's easily
template<typename T, typename priorityT>
struct PriorityQueue
{
	typedef std::pair<priorityT, T> Element;

	// Greater than comparator for the priority queue of pairs
	struct CompareElements
	{
		bool operator()(const Element& e, const Element& e1) const
		{
			return e.first > e1.first;
		}
	};

	// Is this an effeciant way of doing things?
	std::priority_queue<Element, std::vector<Element>, CompareElements> elements;

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

// Order of Coordinates in DIRS array 
// enum Directions N, NW, NE, S, SW, SE, E, W, UP, DOWN
std::array<Coordinates, 10> PathGraph::DIRS{ Coordinates{ 0, -1, 0 },{ -1, -1, 0 },{ 1, -1, 0 },{ 0, 1, 0 },{ -1, 1, 0 },{ 1, 1, 0 },{ 1, 0, 0 },{ -1, 0, 0 },{ 0, 0, 1 },{ 0, 0, -1 } };

// This heuristic needs many more variables eventually!!!
// so user can prioritize pathing for one
inline double heuristic(Coordinates co, Coordinates co1)
{
	return std::abs(co.x - co1.x) + std::abs(co.y - co1.y) + std::abs(co.z - co1.z);
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
	delete tileManager; // Is this needed?
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
		auto& pos = e.getComponent<PositionComponent>();
		auto& mov = e.getComponent<MovementComponent>();

		// If Entity has a destination and doesn't have a
		// path to follow
		if (mov.destination != EMPTY_COORDINATES && mov.path.empty())
		{
			std::unordered_map<Coordinates, Coordinates, CoordinateHash, CoordinateHashEqual> pathMap;
			bool found = aStar(pos.co, mov.destination, pathMap);

			// If path is found
			// Add the path to our Entities MovementComponent
			if(found)
				mov.path = reconstructPath(pos.co, mov.destination, pathMap);

			// Else, remove desination square
			// need to send an update to GUI at somepoint to notify no path found
			else
			{
				mov.destination = EMPTY_COORDINATES;
			}
		}
	}
}

// Test doubles vs doubles in this and helper structs!!
bool MovementAiSystem::aStar(Coordinates start, Coordinates end, std::unordered_map<Coordinates, Coordinates, CoordinateHash, CoordinateHashEqual> &path)
{
	PriorityQueue<Coordinates, double> frontier;
	frontier.put(start, 0);

	std::unordered_map<Coordinates, double, CoordinateHash, CoordinateHashEqual> costSoFar;

	path[start]  = start;
	costSoFar[start] = 0;

	bool pathFound = false;

 	while (!frontier.empty())
	{
		auto current = frontier.get();

		// Add path to entity queue first
		if (current == end)
		{
			pathFound = true;
			break;
		}


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

	return pathFound;
}

std::vector<Coordinates> MovementAiSystem::reconstructPath(Coordinates start, Coordinates end, std::unordered_map<Coordinates, Coordinates, CoordinateHash, CoordinateHashEqual>& cameFrom)
{
	std::vector<Coordinates> path;

	Coordinates current = end;

	// Loop through map path, 
	// starting from the end of the
	// path and ending one tile away from start
	while (current != start)
	{
		path.push_back(current);
		current = cameFrom[current];
	}

	//std::reverse(path.begin(), path.end());

	return path;
}




