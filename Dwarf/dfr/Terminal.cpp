#include "Terminal.h"

#include <SFML/Graphics.hpp>
#include "Textures.h"

namespace dfr
{
	void Terminal::resizePix(const int w, const int h)
	{
		const int x = static_cast<int>(w / (font->characterSize.first));
		const int y = static_cast<int>(h / (font->characterSize.second));

		resizeChars(x, y);
	}

	void Terminal::resizeChars(const int w, const int h)
	{
		dirty = true;

		const int numChars = w * (h + 1);
		buffer.resize(numChars);

		width = w;
		height = h;
		backTex.create(width * font->characterSize.first, height * font->characterSize.second);

		vertices.setPrimitiveType(sf::Quads);

		if (hasBackground)
			vertices.resize(w * h * 8);
		else
			vertices.resize(w * h * 4);

		if (hasBackground)
		{
			const int halfIdx = w * h * 4;
			const int fwidth = font->characterSize.first;
			const int fheight = font->characterSize.second;

			const int spaceX = (219 % 16) * fwidth;
			const int spaceY = (219 / 16) * fheight;

			for (int y = 0; y < h; ++y) {
				for (int x = 0; x < w; ++x) {
					const int idx = ((y * w) + x) * 4;
					const int idx2 = idx + halfIdx;
					vertices[idx].position = sf::Vector2f(static_cast<float>(x * font->characterSize.first), static_cast<float>(y * font->characterSize.second));
					vertices[idx + 1].position = sf::Vector2f(static_cast<float>((x + 1) * font->characterSize.first), static_cast<float>(y * font->characterSize.second));
					vertices[idx + 2].position = sf::Vector2f(static_cast<float>((x + 1) * font->characterSize.first), static_cast<float>((y + 1) * font->characterSize.second));
					vertices[idx + 3].position = sf::Vector2f(static_cast<float>(x * font->characterSize.first), static_cast<float>((y + 1) * font->characterSize.second));

					vertices[idx].texCoords = sf::Vector2f(static_cast<float>(spaceX), static_cast<float>(spaceY));
					vertices[idx + 1].texCoords = sf::Vector2f(static_cast<float>(spaceX + fwidth), static_cast<float>(spaceY));
					vertices[idx + 2].texCoords = sf::Vector2f(static_cast<float>(spaceX + fwidth), static_cast<float>(spaceY + fheight));
					vertices[idx + 3].texCoords = sf::Vector2f(static_cast<float>(spaceX), static_cast<float>(spaceY + fheight));

					vertices[idx2].position = sf::Vector2f(static_cast<float>(x * font->characterSize.first), static_cast<float>(y * font->characterSize.second));
					vertices[idx2 + 1].position = sf::Vector2f(static_cast<float>((x + 1) * font->characterSize.first), static_cast<float>(y * font->characterSize.second));
					vertices[idx2 + 2].position = sf::Vector2f(static_cast<float>((x + 1) * font->characterSize.first), static_cast<float>((y + 1) * font->characterSize.second));
					vertices[idx2 + 3].position = sf::Vector2f(static_cast<float>(x * font->characterSize.first), static_cast<float>((y + 1) * font->characterSize.second));
				}
			}
		}
		else
		{
			for(int x = 0; x < w; ++x)
				for (int y = 0; y < h; ++y)
				{
					const int idx = ((y * w) + x) * 4;
					vertices[idx].position = sf::Vector2f(static_cast<float>(x * font->characterSize.first), static_cast<float>(y * font->characterSize.second));
					vertices[idx + 1].position = sf::Vector2f(static_cast<float>((x + 1) * font->characterSize.first), static_cast<float>(y * font->characterSize.second));
					vertices[idx + 2].position = sf::Vector2f(static_cast<float>((x + 1) * font->characterSize.first), static_cast<float>((y + 1) * font->characterSize.second));
					vertices[idx + 3].position = sf::Vector2f(static_cast<float>(x * font->characterSize.first), static_cast<float>((y + 1) * font->characterSize.second));
				}
		}
	}

	void Terminal::clear() noexcept
	{
		dirty = true;
		std::fill(buffer.begin(), buffer.end(), vchars{ 32, {255, 255, 255}, {0, 0, 0} });
	}

	void Terminal::setChar(const int idx, const vchars & ch) noexcept
	{
		dirty = true;
		if (!(idx < 0 || idx > static_cast<int>(buffer.size())))
			buffer[idx] = ch;
	}

	void Terminal::print(const int x, const int y, const std::string & st, const color & fg, const color & bg) noexcept
	{
	}

	void Terminal::render(sf::RenderWindow & window)
	{
		if (true)
		{
			if (font == nullptr)
				throw std::runtime_error("No Font Loaded!");
			if (texture == nullptr)
				texture = getTexture(font->textureTag);

			const int fwidth = font->characterSize.first;
			const int fheight = font->characterSize.second;

			int vertexIdx = width * height * 4;

			if (!hasBackground) vertexIdx = 0;

			int idx = 0;
			int bgIdx = 0;

			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					const vchars t = buffer[idx];
					const int texX = (t.glyph % 16) * fwidth;
					const int texY = (t.glyph / 16) * fheight;

					if (hasBackground)
					{
						sf::Color bg = colorToSf(t.bg);

						if (alpha == 0 || (t.bg.r == 0 && t.bg.g == 0 && t.bg.b == 0))
							bg.a = 0;
						else
							bg.a = alpha;

						vertices[bgIdx].color = bg;
						vertices[bgIdx + 1].color = bg;
						vertices[bgIdx + 2].color = bg;
						vertices[bgIdx + 3].color = bg;
					}

					vertices[vertexIdx].texCoords = sf::Vector2f(static_cast<int>(texX), static_cast<float>(texY));
					vertices[vertexIdx + 1].texCoords = sf::Vector2f(static_cast<int>(texX + fwidth), static_cast<float>(texY));
					vertices[vertexIdx + 2].texCoords = sf::Vector2f(static_cast<int>(texX + fwidth), static_cast<float>(texY + fheight));
					vertices[vertexIdx + 3].texCoords = sf::Vector2f(static_cast<int>(texX), static_cast<float>(texY + fheight));

					sf::Color fg = colorToSf(t.fg);

					vertices[vertexIdx].color = fg;
					vertices[vertexIdx + 1].color = fg;
					vertices[vertexIdx + 2].color = fg;
					vertices[vertexIdx + 3].color = fg;

					++idx;
					bgIdx += 4;
					vertexIdx += 4;
				}
				backTex.clear(sf::Color(0, 0, 0, 0));
				backTex.draw(vertices, texture);
			}

			backTex.display();
			sf::Sprite comp(backTex.getTexture());
			comp.move(static_cast<float>(offsetX), static_cast<float>(offsetY));
			comp.setColor(sf::Color(tint.r, tint.g, tint.b, alpha));

			window.draw(sf::Sprite(comp));
			dirty = false;
		}
	}
}


