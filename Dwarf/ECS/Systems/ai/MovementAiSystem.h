#pragma once

#include "../../Systems.h"
#include "../../Components/MovementComponent.h"
#include <unordered_map>

#include "../../../Map/Tile.h"

using namespace region;

struct PathGraph
{
	static std::array<Coordinates, 10> DIRS;

	int width, height, depth;

	PathGraph() = default;


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
	inline bool passable(Coordinates start, Coordinates dest) const;


	// Find the neighboring cells and map out 
	// if we have a connecting to them or not
	std::vector<Coordinates> neighbors(Coordinates co) const
	{
		std::vector<Coordinates> result;

		for (auto dir : DIRS)
		{
			Coordinates dest = co;

			dest += dir;

			if (inBounds(dest) && passable(co, dest))
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

	// Helper function for floodFillMap in MovementAiSystem
	std::vector<Coordinates> floodFill(Coordinates co) const;

};



// Handles standard movement for
// Entities
class MovementAiSystem : public System<Requires<MovementComponent>>
{
public:
	MovementAiSystem() = default;
	
	void init();
	void update();

	// Work in progress. Supposed to explore all reachable 
	// areas of map but not discover ones hidden behind rock/etc
	// Not currently working
	void floodFillMap();

	// Search for path with A*, if there is a path return true
	// else return false. 
	// Path is located in path and needs to be reconstructed
	bool aStar(Coordinates start, Coordinates end, std::unordered_map<Coordinates, Coordinates, CoordinateHash, CoordinateHashEqual> &path);

	// Returns a vector of the path in reverse
	// order which will be stored in an entitys movementComponent
	std::vector<Coordinates> reconstructPath(Coordinates start, Coordinates end, std::unordered_map<Coordinates, Coordinates, CoordinateHash, CoordinateHashEqual>& cameFrom);

private:
	PathGraph pathGraph;
};

