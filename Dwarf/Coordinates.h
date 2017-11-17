#pragma once

// Holds what it says, used for 
// index lookups
struct Coordinates {
	int x, y, z;

	// Equality testing for coordinates
	bool operator==(const Coordinates & co)
	{
		return (co.x == x && co.y == y && co.z == z);
	}

	// Adding a Coordiante to another
	void operator+=(const Coordinates & co)
	{
		x += co.x;
		y += co.y;
		z += co.z;
	}
};