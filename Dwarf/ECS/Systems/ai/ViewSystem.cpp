#include "stdafx.h"
#include "ViewSystem.h"
#include "ECS\Messages\entity_moved_message.h"
#include "ECS\Systems\EntityPositionCache.h"
#include "Map\Tile.h"

/*
* Perform a function for each line element between x1/y1/z1 and x2/y2/z2. Uses a 3D
* implementation of Bresenham's line algorithm.
* https://gist.github.com/yamamushi/5823518
* https://github.com/thebracket/bgame/blob/master/src/bengine/geometry.hpp
*/
template<typename T>
void threeDlineFunc(const Coordinates &co1, const Coordinates &co2, T &&func) noexcept
{
	float x = static_cast<float>(co1.x);
	float y = static_cast<float>(co1.y);
	float z = static_cast<float>(co1.z);

	float len = static_cast<float>(get_3D_distance(co1, co2));

	// How many tiles are we going through?
	int steps = static_cast<int>(std::floor(len));

	float xStep = (x - co2.x) / len;
	float yStep = (y - co2.y) / len;
	float zStep = (z - co2.z) / len;

	for (int i = 0; i < steps; ++i)
	{
		x += xStep;
		y += yStep;
		z += zStep;
		const bool keepGoing = func({ static_cast<int>(std::floor(x)), static_cast<int>(std::floor(y)), static_cast<int>(std::floor(z)) });
		if (!keepGoing)
			return;
	}
}

void ViewSystem::init()
{
	subscribe<entity_moved_message>([this](entity_moved_message &msg) // TODO: Make entities dirty after standing still for long enough! Otherwise they'll never see something coming
	{
		dirtyEnts.emplace(msg.e.getId().index);
	});
}

inline void addToView(const int& idx, View &v)
{
	v.visibleCache.emplace_back(idx);
}

inline void calculateView(const Coordinates &co, View &v, int x, int y, int z)   // TODO: Block view of entities with buildings or not?
{
	const float distSqrd = v.radius * v.radius;

	int lastZ = co.z;
	threeDlineFunc(co, { co.x + x, co.y + y, co.z + z }, [&](const Coordinates tmp)
	{
		using region::TileTypes;
		if (tmp.x < 1 || tmp.x > MAP_WIDTH - 1 || tmp.y < 1 || tmp.y > MAP_HEIGHT - 1 || tmp.z < 1 || tmp.z > MAP_DEPTH - 1)
			return false;
		
		const auto idx = getIdx(tmp);
		bool blocked = region::solid(idx); // TODO: Add in opacity if needed

		if (!blocked && lastZ != tmp.z)
		{
			// Ceilings and floors
			if (lastZ > tmp.z && region::getTileType(idx) == TileTypes::FLOOR)
				blocked = true;

			else if (lastZ < tmp.z && region::getTileType(getIdx({ tmp.x, tmp.y, lastZ }) == TileTypes::FLOOR))
				blocked = true;
		}

		if (!blocked || lastZ == tmp.z) // ??? lastZ == tmp.z ???
			addToView(idx, v);

		const float tmpDist = fast_3D_distance(co, tmp);

		if (tmpDist > distSqrd)
			return false;

		lastZ = tmp.z;
		return !blocked;
	});
}

void updateView(const Entity &e, const Coordinates &co, View &v) // TODO: This seems like a system that would be ripe for SMP
{
	v.visibleCache.clear();

	for (int z = 0 - v.radius; z < v.radius; ++z)
		for (int i = 0 - v.radius; i < v.radius; ++i) // TODO: Excessive by about 2x - 4x needed tile hits. 
		{											  // Getting 6 - 10k tiles with 10 raidus on perfectly flat surface
			calculateView(co, v, i, 0 - v.radius, z);
			calculateView(co, v, i,     v.radius, z);
			calculateView(co, v, 0 - v.radius, i, z);
			calculateView(co, v, v.radius    , i, z);
		}
}

void ViewSystem::update(const double duration) 
{
	if (dirtyEnts.empty())
		return;

	for (const auto& e : getEntities()) 
	{
		auto&[view, co] = e.getComponents<View, PositionComponent>();

		// Update view if entity has moved
		// or doesn't have any view cached
		if (view.visibleCache.empty() || dirtyEnts.find(e.getId().index) != dirtyEnts.end())
		{
			updateView(e, co.co, view);

			dirtyEnts.erase(e.getId().index);
		}

		// TODO: Reveal map that hasn't been revealed yet!

		// Loop through all visible tiles
		for (const auto& idx : view.visibleCache)
		{
			// Grab vector of entities at this location
			const auto& visibleHere = positionCache->get_location(idx);

			// Loop through all entities and emplace them
			// into this entities view
			for (const auto& vis : visibleHere)
				view.inView.emplace(vis);
		}
	}

	dirtyEnts.clear();
}
