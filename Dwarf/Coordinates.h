#pragma once
#include "../cereal/archives/binary.hpp"
// Holds what it says, used for 
// index lookups
struct Coordinates 
{
	int x, y, z;

	// Equality testing for coordinates
	bool operator==(const Coordinates & co) const
	{
		return (co.x == x && co.y == y && co.z == z);
	}

	// Inequality testing
	bool operator!=(const Coordinates & co) const
	{
		return (co.x != x || co.y != y || co.z != z);
	}

	// Adding a Coordiantes to another
	void operator+=(const Coordinates & co)
	{
		x += co.x;
		y += co.y;
		z += co.z;
	}

	Coordinates operator+(const Coordinates & co)
	{
		return { x + co.x, y + co.y, z + co.z };
	}

	Coordinates operator-(const Coordinates & co)
	{
		return {x - co.x, y - co.y, z - co.z};
	}

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(x, y, z);
	}
};

// Hashing algorithm for Coordinates
// in unordered_map or other data structures
// Possibly revisit this if performance is an issue,
// this probably isn't a very good hash?
struct CoordinateHash
{
	size_t operator()(const Coordinates & co) const
	{
		return(co.x * 10079 + co.y * 20047) ^ (co.z * 30089);
	}
};

// Equality testing for unordered_map
// or other data structures
struct CoordinateHashEqual
{
	bool operator()(const Coordinates & co, const Coordinates & co1) const
	{
		return (co.x == co1.x && co.y == co1.y && co.z == co1.z);
	}
};

// 0, 0, 0 can never be reached on the map
constexpr Coordinates EMPTY_COORDINATES = { 0, 0, 0 };

// Adjust two coordinates making one smaller or == to the larger one
// so we can loop through them using xyz++
inline void adjustCoordinatesForLoop(Coordinates &small, Coordinates &large)
{
	if (small.x > large.x)
	{
		int x = small.x;
		small.x = large.x;
		large.x = x;
	}

	if (small.y > large.y)
	{
		int y = small.y;
		small.y = large.y;
		large.y = y;
	}

	if (small.z > large.z)
	{
		int z = small.z;
		small.z = large.z;
		large.z = z;
	}
}