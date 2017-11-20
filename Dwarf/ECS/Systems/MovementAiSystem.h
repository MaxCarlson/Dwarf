#pragma once

#include "../Systems.h"
#include "../Components/MovementComponent.h"
#include <unordered_map>

class TileManager;

struct TileConnections
{
	enum Directions
	{
		NONE,
		N,
		NW,
		NE,
		S,
		SW,
		SE,
		E,
		W,
		UP,
		DOWN
	};

	uint8_t neighbors[9];
};

struct PathGraph
{
	static std::array<Coordinates, 10> DIRS;

	int width, height, depth;

	TileManager * tileManager;

	PathGraph() = default;
	PathGraph(TileManager * tileManager) : tileManager(tileManager) {};
	~PathGraph();


	// Bounds checking. This can probably be optomized
	inline bool inBounds(Coordinates co) const
	{
		return 0 <= co.x && co.x < width && 0 <= co.y && co.y < height && co.y < height && 0 <= co.z && co.z < depth;
	}

	// Weight of moving through different tiles
	// Add a cost attribute to tile.properties?
	inline double cost(Coordinates co, Coordinates dest) const
	{
		static double baseCost = 1;
		double cost = baseCost;

		// Diagonal movement
		if (co.x != dest.x && co.y != dest.y)
			cost *= 1.44;

		return cost;
	}

	// This will need to add obstructed, etc at some point
	// Returns whether or not a Coordinate can be passed through
	// This will need to be customized depending on creature type
	// as well as movement type!!!
	inline bool passable(Coordinates co) const;


	// Find the neighboring cells and map out 
	// if we have a connecting to them or not
	std::vector<Coordinates> neighbors(Coordinates co) const
	{
		std::vector<Coordinates> result;

		for (auto dir : DIRS)
		{
			Coordinates dest = co;

			dest += dir;

			if (inBounds(dest) && passable(dest))
			{
				result.push_back(dest);
			}

			/*
			if ((x + y) % 2 == 0) {
				// aesthetic improvement on square grids
				std::reverse(results.begin(), results.end());
			}
			*/
		}
		return result;
	}
};



// Handles standard movement for
// Entities
class MovementAiSystem : public System<Requires<MovementComponent>>
{
public:
	MovementAiSystem() = default;
	MovementAiSystem(TileManager * tileManager);
	~MovementAiSystem();

	void initMap(TileManager * tileMan);

	void update();

	void aStar(Coordinates start, Coordinates end, std::unordered_map<Coordinates, Coordinates, CoordinateHash, CoordinateHashEqual> &path);

	// Returns a vector of the path in reverse
	// order which will be stored in an entitys movementComponent
	std::vector<Coordinates> reconstructPath(Coordinates start, Coordinates end, std::unordered_map<Coordinates, Coordinates, CoordinateHash, CoordinateHashEqual>& cameFrom);

private:
	TileManager * tileManager;
	PathGraph pathGraph;
};

