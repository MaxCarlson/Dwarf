#pragma once
#include "colors.h"

struct vchars
{
	vchars() = default;

	vchars(uint32_t glyph, color fg, color bg) : glyph(glyph), fg(fg), bg(bg) {}
	
	uint32_t glyph;
	color fg;
	color bg;
};

