#include "NoiseHelper.h"
#include "Map\Tile.h"
#include "Map\World\Planet.h"

double noiseX(int worldX, int regionX)
{
	const float big_x = (worldX * WORLD_WIDTH) + regionX;
	return (big_x / (static_cast<double>(WORLD_WIDTH) * static_cast<double>(MAP_WIDTH))) * NOISE_SIZE;
}

double noiseY(int worldY, int regionY)
{
	const float big_y = (worldY * WORLD_HEIGHT) + regionY;
	return (big_y / (static_cast<double>(WORLD_HEIGHT) * static_cast<double>(MAP_HEIGHT))) * NOISE_SIZE;
}
