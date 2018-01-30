#pragma once
#include <string>

namespace dfr
{
	struct Font
	{
		Font(const std::string textureTag, const int width, const int height) : textureTag(textureTag), characterSize({ width, height }) {}
		const std::string textureTag;
		const std::pair<int, int> characterSize;
	};

	void registerFont(const std::string tag, const std::string filePath, int width = 16, int height = 16);
	Font * getFont(const std::string tag);

}


