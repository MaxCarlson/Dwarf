#pragma once
#include <vector>
#include "../Coordinates.h"

struct CacheLocation
{
	CacheLocation(Coordinates co, std::size_t id) : co(co), id(id) {}
	//CacheLocation(int idx, std::size_t id);
	Coordinates co;
	std::size_t id;
};

class PositionCache
{
public:
	PositionCache();


	std::vector<std::vector<std::size_t>> entrys;

	std::size_t totalNodes = 0;

	void addNode(const CacheLocation & pos);
	void removeNode(const CacheLocation & pos);

	const std::vector<std::size_t>& get_location(const Coordinates co);
	const std::vector<std::size_t>& get_location(int idx);

	std::vector<std::size_t> find_by_region(const int left, const int right, const int top, const int bottom, const int ztop, const int zbottom);
};

