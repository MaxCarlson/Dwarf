#pragma once
#include <string>
#include <vector>
#include <cereal/cereal.hpp>

extern int WORLD_WIDTH  = 50;
extern int WORLD_HEIGHT = 50;

enum PlanetTileType
{
	PLAINS = 1,
	HILLS = 2,
	MOUNTAINS = 3,
	PLATEAUS = 4,
	OCEANS = 5,
	COASTS = 6
};

struct tile_p
{
	int height;
	int temperature;
	int rainfall;

	int biomeIdx;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(height, temperature, rainfall, biomeIdx);
	}
};

struct Biome
{
	std::size_t id;
	std::string name;

	int avgAltitude;
	int avgRainfall;
	int avgTemperature;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(id, name, avgAltitude, avgRainfall, avgTemperature);
	}
};

struct Planet
{
	int dwarvesNumber = 7;
	int userSeed = 1;
	int noiseSeed = 1;

	int waterLevel = 1;

	int waterFreq = 1;
	int plainsFreq = 1;
	int hillsFreq = 1;

	std::vector<Biome> biomes;
	std::vector<tile_p> tiles;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(dwarvesNumber, userSeed, noiseSeed, waterLevel, waterFreq, plainsFreq, hillsFreq, biomes, tiles);
	}
};

