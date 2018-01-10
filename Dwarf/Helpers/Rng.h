#pragma once
#include <string>
#include "pcg_basic.h"


class Rng
{
public:
	Rng();
	Rng(const int seed);
	Rng(const std::string seedString);

	int range(const int alpha, const int beta);

	int seed;

private:
	pcg32_random_t rng;
};

