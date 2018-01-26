#pragma once

struct Planet;
struct Coordinates;
class Rng;

void buildRegion(Planet &planet, int px, int py, Coordinates dimensions, Rng &rng);
