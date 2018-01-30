#pragma once
#include "FontManager.h"
#include "colors.h"
#include "vchars.h"
#include <SFML\Graphics.hpp>

namespace dfr
{
	class Terminal
	{
	public:
		Terminal(const std::string fontt, const int x = 0, const int y = 0, const bool background = true) : fontTag(fontt), offsetX(x), offsetY(y), hasBackground(background)
		{
			font = getFont(fontt);
		}

		void resizeChars(const int w, const int h);
		void resizePix(const int w, const int h);

		void clear() noexcept;

		inline int at(const int x, const int y) noexcept { return (y * width) + x; }

		void setChar(const int idx, const vchars& ch) noexcept;

		void setChar(const int x, const int y, vchars ch) noexcept { setChar(at(x, y), ch); }

		void print(const int x, const int y, const std::string &st, const color &fg = { 255, 255, 255 }, const color &bg = { 0, 0, 0 }) noexcept;

		void render(sf::RenderWindow &window);

		int width;
		int height;
		bool dirty = true;

	private:
		int offsetX;
		int offsetY;
		bool hasBackground;

		uint8_t alpha = 255;
		sf::VertexArray vertices;
		sf::RenderTexture backTex;
		sf::Texture * texture = nullptr;
		Font * font = nullptr;
		std::string fontTag;

		color tint{ 255, 255, 255 };

		std::vector<vchars> buffer;
	};

}


