#pragma once

// Holds what it says, used for 
// index lookups
struct Coordinates {
	int x, y, z;

	// Equality testing for coordinates
	bool operator==(const Coordinates & cor)
	{
		return (cor.x == x && cor.y == y && cor.z == z);
	}
};