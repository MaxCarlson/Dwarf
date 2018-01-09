#pragma once
#include <string>
#include <vector>
#include <cereal/cereal.hpp>

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
	int userSeed = 1;
	int noiseSeed = 1;


	int waterLevel = 1;

	std::vector<Biome>   biomes;
	std::vector<tile_p> terrain;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(userSeed, noiseSeed, waterLevel, biomes, terrain);
	}
};

