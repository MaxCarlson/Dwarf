#include "Textures.h"
#include <unordered_map>


namespace dfr
{
	std::unordered_map<std::string, Texture> texMap;

	void registerTexture(const std::string path, const std::string tag)
	{
		auto find = texMap.find(tag);

		if (find != texMap.end())
			throw std::runtime_error("Texture tag already in use!");

		texMap[tag] = Texture(path);
	}

	sf::Texture * getTexture(const std::string & tag)
	{
		auto find = texMap.find(tag);

		if (find != texMap.end())
			return find->second.texture.get();

		return nullptr;
	}

}
