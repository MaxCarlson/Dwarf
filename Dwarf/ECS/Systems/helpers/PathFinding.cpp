#include "../stdafx.h"
#include "PathFinding.h"
#include "../Map/Tile.h"


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

inline void findNeighbors(const Coordinates co, std::vector<Coordinates> & points)
{
	points.clear();

	if (flag(co, CAN_GO_NORTH))
		points.emplace_back(CO_NORTH);
	if (flag(co, CAN_GO_SOUTH))
		points.emplace_back(CO_SOUTH);
	if (flag(co, CAN_GO_EAST))
		points.emplace_back(CO_EAST);
	if (flag(co, CAN_GO_WEST))
		points.emplace_back(CO_WEST);
	if (flag(co, CAN_GO_NORTH_W))
		points.emplace_back(CO_NORTH_W);
	if (flag(co, CAN_GO_NORTH_E))
		points.emplace_back(CO_NORTH_E);
	if (flag(co, CAN_GO_SOUTH_W))
		points.emplace_back(CO_SOUTH_W);
	if (flag(co, CAN_GO_SOUTH_E))
		points.emplace_back(CO_SOUTH_E);
	if (flag(co, CAN_GO_UP))
		points.emplace_back(CO_UP);
	if (flag(co, CAN_GO_DOWN))
		points.emplace_back(CO_DOWN);
}

inline double cost(const Coordinates co, const Coordinates dest)
{
	static double baseCost = 1.0;

	if (co.x != dest.x && co.y != dest.y)
		return 1.44;

	return baseCost;
}

// Test doubles vs doubles in this and helper structs!!
bool aStar(Coordinates start, Coordinates end, std::unordered_map<Coordinates, Coordinates, CoordinateHash, CoordinateHashEqual> &path)
{
	PriorityQueue<Coordinates, double> frontier;
	frontier.put(start, 0);

	static std::unordered_map<Coordinates, double, CoordinateHash, CoordinateHashEqual> costSoFar;
	costSoFar.clear();

	path[start] = start;
	costSoFar[start] = 0;

	bool pathFound = false;

	static std::vector<Coordinates> neighbors;
	neighbors.clear();


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

std::vector<Coordinates> reconstructPath(Coordinates start, Coordinates end, std::unordered_map<Coordinates, Coordinates, CoordinateHash, CoordinateHashEqual>& cameFrom)
{
	static std::vector<Coordinates> path;
	path.clear();

	Coordinates current = end;

	// Loop through map path, 
	// starting from the end of the
	// path and ending one tile away from start
	while (current != start)
	{
		path.emplace_back(current);
		current = cameFrom[current];
	}

	//std::reverse(path.begin(), path.end());

	return path;
}

std::unique_ptr<Path> findPath(Coordinates start, Coordinates dest)
{
	static std::unordered_map<Coordinates, Coordinates, CoordinateHash, CoordinateHashEqual> pathMap;
	pathMap.clear();

	std::unique_ptr<Path> path = std::make_unique<Path>();

	path->failed = !aStar(start, dest, pathMap);

	if(!path->failed)
		path->path = reconstructPath(start, dest, pathMap);

	return path;
}
