#include "FontManager.h"
#include "Textures.h"
#include <unordered_map>

namespace dfr
{
	std::unordered_map<std::string, Font> fontMap;

	void registerFont(const std::string tag, const std::string filePath, int width, int height)
	{
		const std::string texTag = "font" + filePath;

		registerTexture(filePath, texTag);
		fontMap.emplace(std::make_pair(tag, Font(texTag, width, height)));
	}


	Font * getFont(const std::string tag)
	{
		auto find = fontMap.find(tag);

		if (find != fontMap.end())
			return &find->second;

		return nullptr;
	}
}


