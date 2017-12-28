#pragma once
#include <string>


struct vchar
{
	vchar() = default;
	vchar(int c, uint32_t fg, uint32_t bg) : c(c), fg(fg), bg(bg) {}
	int c;
	uint32_t fg; // Should these be made into ints / chars indexing the actualy colors? or just uint32_t's for full alpha color with bearlib?
	uint32_t bg;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(c, fg, bg);
	}
	// Possibly add a tileset code? Although really tilesets should just be loaded into memory and indexed from there
};