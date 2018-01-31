#pragma once
#include "Coordinates.h"
#include "Drawing\vchar.h"
#include <string>
#include <vector>
#include <mutex>
#include "../World/Planet.h"

extern Planet planet;

extern std::mutex planetBuilderLock;
extern std::vector<vchar> worldGenDisplay;
extern std::atomic<bool> worldGenDone;

void updateWorldDisplay(Planet & planet);

void buildPlanet(const std::string seed, const int pwidth, const int pheight, Coordinates mapxyz, const int water, const int plains, const int numDwarves);

