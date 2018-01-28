#pragma once
#include "Coordinates.h"
#include <string>

extern std::unique_ptr<std::thread> worldThread;

void generateWorld(const std::string seed, const int pwidth, const int pheight, Coordinates mapxyz, const int water, const int plains, const int numDwarves);

bool chooseEmbark(int &px, int &py);