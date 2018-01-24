#pragma once

constexpr int MAP_FRACTION = 64;
constexpr int NOISE_SIZE = 166; // 166
constexpr int R_SMOOTHNESS = 37; // Smoothness
constexpr unsigned int octaves = 7;
constexpr float persistence = 0.5F;
constexpr float frequency = 2.0F;

double noiseX(int worldX, int regionX);
double noiseY(int worldY, int regionY);


inline uint8_t noiseToHeight(const double n)
{
	return static_cast<uint8_t>((n + 1.0f) * 123);
}