#pragma once
#include <SFML\Graphics.hpp>

struct color
{
public:
	color() = default;
	color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
	color(const uint32_t c) 
	{
		r = (c >> 16) & 0xFF;
		g = (c >> 8) & 0xFF;
		b = c & 0xFF;
	}
	
	uint8_t r, g, b;
};

inline sf::Color colorToSf(const color &col) { return sf::Color(col.r, col.g, col.b); }
color lerp(const color &first, const color &second, float amount);

