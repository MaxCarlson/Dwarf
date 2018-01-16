#include "stdafx.h"
#include "Rng.h"
#include <ctime>

Rng rng;

Rng::Rng()
{
	pcg32_srandom_r(&rng, time(NULL), (intptr_t)&rng);
}

Rng::Rng(const int seed)
{
	pcg32_srandom_r(&rng, seed, (intptr_t)&rng);
}

Rng::Rng(const std::string seedString)
{
	std::hash<std::string> hashFunc;
	seed = static_cast<int>(hashFunc(seedString));
	pcg32_srandom_r(&rng, seed, (intptr_t)&rng);
}

int Rng::range(const int alpha, const int beta)
{
	return pcg32_boundedrand_r(&rng, beta) + alpha;
}


