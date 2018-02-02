#pragma once
#include <cereal\cereal.hpp>


struct Camera
{
	int x = 0;
	int y = 0;
	int z = 0;

	int zoom = 10;

	int offsetX = 0;
	int offsetY = 0;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(x, y, z, zoom, offsetX, offsetY);
	}
};

extern Camera camera;

