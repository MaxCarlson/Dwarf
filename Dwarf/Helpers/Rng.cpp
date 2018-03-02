#include "stdafx.h"
#include "Rng.h"
#include <ctime>

Rng rng;

inline int hashString(const std::string &str)
{
	std::hash<std::string> hashFunc;
	return static_cast<int>(hashFunc(str));
}

Rng::Rng() : stdRng(0)
{
	pcg32_srandom_r(&rng, time(NULL), (intptr_t)&rng);
}

Rng::Rng(const int seed) : stdRng(seed)
{
	pcg32_srandom_r(&rng, seed, (intptr_t)&rng);
}

Rng::Rng(const std::string seedString) : stdRng(hashString(seedString))
{
	seed = hashString(seedString);
	pcg32_srandom_r(&rng, seed, (intptr_t)&rng);
}

int Rng::range(const int alpha, const int beta)
{
	return pcg32_boundedrand_r(&rng, beta) + alpha;
}

double Rng::range(const double lower, const double upper)
{
	std::uniform_real_distribution<double> unif(lower, upper);
	return unif(stdRng);
}


