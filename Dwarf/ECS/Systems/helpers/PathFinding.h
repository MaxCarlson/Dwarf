#pragma once
#include "../../../Coordinates.h"

struct Path
{
	bool failed = false;

	std::vector<Coordinates> path;
};


std::unique_ptr<Path> findPath(Coordinates start, Coordinates dest);

