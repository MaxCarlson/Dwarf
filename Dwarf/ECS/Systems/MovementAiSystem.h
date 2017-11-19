#pragma once

#include "../Systems.h"
#include "../Components/MovementComponent.h"
#include <unordered_set>
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

	//std::unordered_set<Coordinates> walls;

	// Use this 
	//std::unordered_set<Coordinates> obstructed;

	PathGraph() = default;
	PathGraph(TileManager * tileManager) : tileManager(tileManager) {};
	~PathGraph() { delete tileManager; }


	// Bounds checking. This can probably be optomized
	inline bool inBounds(Coordinates co) const
	{
		return 0 <= co.x && co.x < width && 0 <= co.y && co.y < height && co.y < height && 0 <= co.z && co.z < depth;
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
	}
private:
};
// Order of Coordinates in DIRS array below
// enum Directions N, NW, NE, S, SW, SE, E, W, UP, DOWN
std::array<Coordinates, 10> PathGraph::DIRS{ Coordinates{0, -1, 0}, {-1, -1, 0}, {1, -1, 0},  {0, 1, 0}, {-1, 1, 0}, {1, 1, 0}, {1, 0, 0}, {-1, 0, 0}, {0, 0, 1}, {0, 0, -1} };

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

private:
	TileManager * tileManager;
	PathGraph pathGraph;
};

