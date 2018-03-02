#pragma once
#include <string>
#include "pcg_basic.h"
#include <random>

class Rng
{
public:
	Rng();
	Rng(const int seed);
	Rng(const std::string seedString);

	int range(const int alpha, const int beta);

	double range(const double lower, const double upper);

	int seed;

private:
	pcg32_random_t rng;
	std::default_random_engine stdRng;
};

extern Rng rng;

