#pragma once
#include <SFML\Graphics.hpp>
#include <queue>

namespace dfr
{
	enum Button
	{
		LEFT,
		RIGHT,
		MIDDLE,
		SIDE1,
		SIDE2,
		WHEEL_UP,
		WHEEL_DOWN
	};

	void initInputs(const int fontSizeX, const int fontSizeY);

	extern bool isWindowFocused();

	extern void setWindowFocus(const bool f);

	extern void resetMouseState();

	extern void setMousePosition(const int x, const int y);

	extern std::pair<int, int> getMousePosition();

	extern std::pair<int, int> getMousePositionPx();

	extern void setMouseState(const int button, const bool state);
	extern bool getMouseState(const int button);

	extern void keyPressed(sf::Event &event);

	struct KeyPressed
	{
		KeyPressed() = default;
		KeyPressed(sf::Event e) : event(e) {}

		sf::Event event;
	};

	extern std::queue<sf::Event> & readInput();

}

