#pragma once
#include <SFML\Graphics.hpp>

struct color
{
public:
	color() = default;
	color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
	
	uint8_t r, g, b;
};

inline sf::Color colorToSf(const color &col) { return sf::Color(col.r, col.g, col.b); }

