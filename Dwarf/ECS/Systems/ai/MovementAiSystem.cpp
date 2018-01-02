#include "MovementAiSystem.h"
#include "../Map/Tile.h"
#include "../../Components/PositionComponent.h"
#include "../../Components/JobComponent.h"
#include <queue>
#include <functional>
#include <utility>


using namespace region;

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

// This heuristic needs many more variables eventually!!!
// so user can prioritize pathing for one
inline double heuristic(Coordinates co, Coordinates co1)
{
	return std::abs(co.x - co1.x) + std::abs(co.y - co1.y) + std::abs(co.z - co1.z);
}

void MovementAiSystem::update()
{
	const auto& entities = getEntities();

	for (auto& e : entities)
	{
		auto& pos = e.getComponent<PositionComponent>();
		auto& mov = e.getComponent<MovementComponent>();

		// If Entity has a destination and doesn't have a
		// path to follow compute path and store in MovementComponent
		if (mov.destination != EMPTY_COORDINATES && mov.path.empty())
		{
			std::unordered_map<Coordinates, Coordinates, CoordinateHash, CoordinateHashEqual> pathMap;
			bool found = aStar(pos.co, mov.destination, pathMap);

			// If path is found
			// Add the path to our Entities MovementComponent
			if (found)
			{
				mov.path = reconstructPath(pos.co, mov.destination, pathMap);
				mov.cannotFindPath = false;
			}
			// Else, remove desination square
			// need to send an update to GUI at somepoint to notify no path found
			else
			{
				mov.destination = EMPTY_COORDINATES;
				mov.cannotFindPath = true;
			}
		}
	}
}

inline void findNeighbors(const Coordinates co, std::vector<Coordinates> & points)
{
	points.clear();

	if (flag(co, CAN_GO_NORTH))
		points.push_back(CO_NORTH);
	if (flag(co, CAN_GO_SOUTH))
		points.push_back(CO_SOUTH);
	if (flag(co, CAN_GO_EAST))
		points.push_back(CO_EAST);
	if (flag(co, CAN_GO_WEST))
		points.push_back(CO_WEST);
	if (flag(co, CAN_GO_NORTH_W))
		points.push_back(CO_NORTH_W);
	if (flag(co, CAN_GO_NORTH_E))
		points.push_back(CO_NORTH_E);
	if (flag(co, CAN_GO_SOUTH_W))
		points.push_back(CO_SOUTH_W);
	if (flag(co, CAN_GO_SOUTH_E))
		points.push_back(CO_SOUTH_E);
	if (flag(co, CAN_GO_UP))
		points.push_back(CO_UP);
	if (flag(co, CAN_GO_DOWN))
		points.push_back(CO_DOWN);
}

inline double cost(const Coordinates co, const Coordinates dest)
{
	static double baseCost = 1.0;

	if (co.x != dest.x && co.y != dest.y)
		return 1.44;

	return baseCost;
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

	std::vector<Coordinates> neighbors;
	neighbors.reserve(10);

 	while (!frontier.empty())
	{
		auto current = frontier.get();

		// Add path to entity queue first
		if (current == end)
		{
			pathFound = true;
			break;
		}

		findNeighbors(current, neighbors);

		// Loop through all neighbors of currrent tile
		for (auto& next : neighbors)//pathGraph.neighbors(current))
		{
			double newCost = costSoFar[current] + cost(current, next);

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




