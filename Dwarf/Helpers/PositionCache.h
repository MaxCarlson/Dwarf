#pragma once
#include <vector>
#include "../Coordinates.h"

struct CacheLocation
{
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

	std::vector<std::size_t> get_location(const Coordinates co);
	std::vector<std::size_t> get_location(int idx);
};

