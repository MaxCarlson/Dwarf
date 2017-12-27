#pragma once
#include <string>

struct vchar
{
	int c;
	std::string fg; // Should these be made into ints / chars indexing the actualy colors? or just uint32_t's for full alpha color with bearlib?
	std::string bg;

	// Possibly add a tileset code? Although really tilesets should just be loaded into memory and indexed from there
};