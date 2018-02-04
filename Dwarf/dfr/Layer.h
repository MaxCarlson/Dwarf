#pragma once

#include "Terminal.h"
#include <memory>
#include <unordered_map>
#include <functional>

namespace dfr
{


struct Layer
{

	Layer(const int x, const int y, const int w, const int h, std::string fontTag, std::function<void(Layer *, int, int)> resizeFunc, bool hasBackground = true)
		: x(x), y(y), w(w), h(h), fontTag(fontTag), resizeFunc(resizeFunc), hasBackground(hasBackground) 
	{
		terminal = std::make_unique<Terminal>(fontTag, x, y, hasBackground);
		terminal->resizePix(w, h);
	}

	// Box the layer is defined by
	int x;
	int y;
	int w;
	int h;

	std::string fontTag;

	std::function<void(Layer *, int, int)> resizeFunc;

	bool hasBackground;

	std::unique_ptr<Terminal> terminal;
	std::unique_ptr<sf::RenderTexture> back;

	void render(sf::RenderWindow &window);
	void onResize(const int width, const int height);
};



} // End namespace



