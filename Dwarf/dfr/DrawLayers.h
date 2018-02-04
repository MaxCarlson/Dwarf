#pragma once

#include "Layer.h"
#include <unordered_map>

namespace dfr
{

struct DrawLayers
{
	DrawLayers(const int w, const int h) : width(w), height(h) {}

	void onResize(const int w, const int h);
	void render(sf::RenderWindow &window);
	void clearAllLayers();

	void addLayer(const int id, const int x, const int y, const int w, const int h, std::string fontTag, std::function<void(Layer *, int, int)> resizeFunc, bool hasBackgound = true, int order = -1);

	void deleteLayer(const int id);
	Layer * getLayer(const int id);

private:
	int width;
	int height;
	int renderOrder = 0;

	std::unordered_map<int, Layer> layers;

	inline void checkUniqueLayer(const int l)
	{
		auto find = layers.find(l);
		if (find != layers.end())
			throw std::runtime_error("Layer Id Already Used!");
	}
};

} // End namepsace


