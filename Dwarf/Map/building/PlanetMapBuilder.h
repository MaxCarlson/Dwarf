#pragma once
#include "Helpers\FastNoise.h"

struct Planet;
struct Rng;

FastNoise genPlanetNoiseMap(Planet &planet);

void builPlanetTileTypes(Planet &planet);

void drawCoastlines(Planet &planet);

void buildPlanetRainfall(Planet &planet);

void buildPlanetBiomes(Planet &planet, Rng &rng);