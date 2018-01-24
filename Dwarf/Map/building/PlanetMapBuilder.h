#pragma once
#include "Helpers\FastNoise.h"

struct Planet;

FastNoise genPlanetNoiseMap(Planet &planet);

void builPlanetTileTypes(Planet &planet);