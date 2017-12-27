#include "PositionCache.h"
#include "../Map/Tile.h"
#include <algorithm>

/*
CacheLocation::CacheLocation(int idx, std::size_t id)
{
	co = idxToCo(idx);
	id = id;
}
*/
PositionCache::PositionCache()
{
	entrys.resize(TOTAL_MAP_TILES);
}

void PositionCache::addNode(const CacheLocation & pos)
{
	const int idx = getIdx(pos.co);
	entrys.emplace_back(pos.id);
	++totalNodes;
}

void PositionCache::removeNode(const CacheLocation & pos)
{
	const int idx = getIdx(pos.co);

	// Remove any entries at position idx that match (Entity) id's
	entrys[idx].erase(
		std::remove_if(
			entrys[idx].begin(),
			entrys[idx].end(),
			[&pos](const std::size_t & var) { return var == pos.id; }

		),
		entrys[idx].end()
	);
}

std::vector<std::size_t> PositionCache::get_location(const Coordinates co)
{
	return get_location(getIdx(co));
}

std::vector<std::size_t> PositionCache::get_location(int idx)
{
	std::vector<std::size_t> loc;
	for (const auto& p : entrys[idx])
	{
		loc.push_back(p);
	}
	return loc;
}

std::vector<std::size_t> PositionCache::find_by_region(const int left, const int right, const int top, const int bottom, const int ztop, const int zbottom)
{
	std::vector<std::size_t> loc;

	for (int z = zbottom; z < ztop; ++z) // Possibly change z order?
		for(int x = left; x < right; ++x)
			for (int y = top; y < bottom; ++y)
			{
				for (const auto& ent : entrys[getIdx({ x, y, z })])
				{
					loc.push_back(ent);
				}
			}
	return loc;
}
