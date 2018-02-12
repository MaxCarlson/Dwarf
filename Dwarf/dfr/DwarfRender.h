#pragma once
#include "Terminal.h"
#include "colors.h"
#include "Textures.h"
#include "FontManager.h"
#include "Inputs.h"
#include "DrawLayers.h"
#include <string>
#include <memory>
#include <functional>
#include <SFML\Graphics.hpp>


namespace dfr
{
	struct config_r
	{
		config_r(const std::string title, const std::string fontPath, int width, int height, const std::string fontTag) 
			: title(title), fontPath(fontPath), width(width), height(height), fontTag(fontTag) {}

		const std::string title;
		const std::string fontPath;
		const int width;
		const int height;
		const std::string fontTag;
	};

	extern std::unique_ptr<Terminal> terminal;

	sf::RenderWindow * getWindow();

	extern std::unique_ptr<DrawLayers> gui;

	void init(config_r config);

	void run(std::function<void(double)>& onTick);

	inline Layer * layer(const int id) { return gui->getLayer(id); }
	inline Terminal * term(const int id) { return gui->getLayer(id)->terminal.get(); }

	void takeScreenShot(std::string fileName);
}

