#pragma once

#include "../Systems.h"
#include "../Components/MovementComponent.h"

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

template<typename L>
struct SimpleGraph {
	typedef L Location;
	typedef typename vector<Location>::iterator iterator;
	unordered_map<Location, vector<Location> > edges;

	inline const vector<Location>  neighbors(Location id) {
		return edges[id];
	}
};

using namespace std;
#include <unordered_set>

struct SquareGrid {
	typedef tuple<int, int> Location;
	static array<Location, 4> DIRS;

	int width, height;
	unordered_set<Location> walls;

	SquareGrid(int width_, int height_)
		: width(width_), height(height_) {}

	inline bool in_bounds(Location id) const {
		int x, y;
		tie(x, y) = id;
		return 0 <= x && x < width && 0 <= y && y < height;
	}

	inline bool passable(Location id) const {
		return !walls.count(id);
	}

	vector<Location> neighbors(Location id) const {
		int x, y, dx, dy;
		tie(x, y) = id;
		vector<Location> results;

		for (auto dir : DIRS) {
			tie(dx, dy) = dir;
			Location next(x + dx, y + dy);
			if (in_bounds(next) && passable(next)) {
				results.push_back(next);
			}
		}

		if ((x + y) % 2 == 0) {
			// aesthetic improvement on square grids
			std::reverse(results.begin(), results.end());
		}

		return results;
	}
};
array<SquareGrid::Location, 4> SquareGrid::DIRS{ Location{ 1, 0 }, Location{ 0, -1 }, Location{ -1, 0 }, Location{ 0, 1 } };

// Handles standard movement for
// Entities
class MovementAiSystem : public System<Requires<MovementComponent>>
{
public:
	MovementAiSystem();
	~MovementAiSystem();

	void initMap(const TileManager & tileManager);

	void update();

private:

};

