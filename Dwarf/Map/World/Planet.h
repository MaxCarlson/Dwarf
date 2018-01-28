#pragma once
#include <string>
#include <vector>
#include <cereal/cereal.hpp>

extern int WORLD_WIDTH;
extern int WORLD_HEIGHT;

enum PlanetTileType
{
	MAX_PLANET_TILE_TYPES = 7,
	NONE = 0,
	WATER = 1,
	PLAINS = 2,
	HILLS = 3,
	MOUNTAINS = 4,
	HIGHLANDS = 5,
	PLATEAUS = 6,
	COASTS = 7
};

struct tile_p
{
	int type = 0;
	int height = 0;
	int variance = 0;
	int temperature = 0;
	int rainfall = 0;

	int biomeIdx = -1;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(type, height, temperature, rainfall, biomeIdx);
	}
};

struct Biome
{
	std::size_t type;
	std::string name;

	int centerX;
	int centerY;

	int avgAltitude;
	int avgRainfall;
	int avgTemperature;
	int avgVariance;

	int savagery;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(type, name, centerX, centerY, avgAltitude, avgRainfall, avgTemperature, avgVariance, savagery);
	}
};

struct Planet
{
	int dwarvesNumber = 7;
	int userSeed = 1;
	int noiseSeed = 1;

	int waterLevel = 1;
	int plainsHeight = 1;
	int hillsHeight = 1;

	int waterFreq = 1;
	int plainsFreq = 1;
	int hillsFreq = 1;

	std::vector<Biome> biomes;
	std::vector<tile_p> tiles;

	inline int idx(int x, int y) const { return y * WORLD_WIDTH + x; }

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(dwarvesNumber, userSeed, noiseSeed, waterLevel, plainsHeight, hillsHeight, waterFreq, plainsFreq, hillsFreq, biomes, tiles);
	}
};

