#include "colors.h"

color lerp(const color &first, const color &second, float amount) 
{
	const float r1 = first.r;
	const float g1 = first.g;
	const float b1 = first.b;

	const float r2 = second.r;
	const float g2 = second.g;
	const float b2 = second.b;

	const float rdiff = r2 - r1;
	const float gdiff = g2 - g1;
	const float bdiff = b2 - b1;

	float red	= r1 + (rdiff * amount);
	float green = g1 + (gdiff * amount);
	float blue	= b1 + (bdiff * amount);
	if (red		> 255.0F)	red		= 255.0F;
	if (green	> 255.0F)	green	= 255.0F;
	if (blue	> 255.0F)	blue	= 255.0F;
	if (red		< 0.0F)		red		= 0.0F;
	if (green	< 0.0F)		green	= 0.0F;
	if (blue	< 0.0F)		blue	= 0.0F;

	const int r = static_cast<const int>(red);
	const int g = static_cast<const int>(green);
	const int b = static_cast<const int>(blue);

	return color(r, g, b);
}

