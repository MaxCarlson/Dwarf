#pragma once
#include <SFML\Graphics.hpp>
#include <memory>
#include <string>

namespace dfr
{
	struct Texture
	{
		Texture() = default;
		Texture(const std::string &fileName) 
		{
			texture = std::make_unique<sf::Texture>();
			if (!texture->loadFromFile(fileName))
				throw std::runtime_error("Inproper file name for texture!");
		}

		std::unique_ptr<sf::Texture> texture;
	};

	void registerTexture(const std::string path, const std::string tag);
	sf::Texture * getTexture(const std::string &tag);
}

