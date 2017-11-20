#pragma once

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

	bool operator!=(const Coordinates & co) const
	{
		return (co.x != x || co.y != y || co.z != z);
	}

	// Adding a Coordiante to another
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

	bool operator<(const Coordinates & co) const;
};

struct CoordinateHash
{
	size_t operator()(const Coordinates & co) const
	{
		return(co.x * 10079 + co.y * 20047) ^ (co.z * 30089);
	}
};

struct CoordinateHashEqual
{
	bool operator()(const Coordinates & co, const Coordinates & co1) const
	{
		return (co.x == co1.x && co.y == co1.y && co.z == co1.z);
	}
};

// Probably won't ever have maps this large!!
const static Coordinates EMPTY_COORDINATES = { 10000, 10000, 10000 };

/*
namespace std
{
	template <>
	struct hash<Key>
	{
		std::size_t operator()(const Key& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:
			
			return ((hash<string>()(k.first)
				^ (hash<string>()(k.second) << 1)) >> 1)
				^ (hash<int>()(k.third) << 1);
		}
	};
}
*/