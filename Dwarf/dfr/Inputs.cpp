#include "Inputs.h"
#include <array>

namespace dfr
{
	std::queue<sf::Event> events;
	int fontX;
	int fontY;

	namespace mstate
	{
		bool isWindowFocused = true;
		std::array<bool, 7> mouseButtonsPressed;
		int mouseX = 0;
		int mouseY = 0;
	}

	void initInputs(const int fontSizeX, const int fontSizeY)
	{
		fontX = fontSizeX;
		fontY = fontSizeY;
	}

	bool isWindowFocused()
	{
		return mstate::isWindowFocused;
	}

	void setWindowFocus(const bool f)
	{
		mstate::isWindowFocused = f;
	}

	void resetMouseState()
	{
		std::fill(mstate::mouseButtonsPressed.begin(), mstate::mouseButtonsPressed.end(), false);
		mstate::mouseX = 0;
		mstate::mouseY = 0;
	}

	void setMousePosition(const int x, const int y)
	{
		mstate::mouseX = x;
		mstate::mouseY = y; 
	}

	std::pair<int, int> getMousePosition()
	{
		return std::make_pair(mstate::mouseX / fontX, mstate::mouseY / fontY);
	}

	std::pair<int, int> getMousePositionPx()
	{
		return std::make_pair(mstate::mouseX, mstate::mouseY);
	}

	void setMouseState(const int button, const bool state)
	{
		mstate::mouseButtonsPressed[button] = state;
	}

	bool getMouseState(const int button)
	{
		return mstate::mouseButtonsPressed[button];
	}

	void keyPressed(sf::Event & event)
	{
		events.push(event);
	}

	std::queue<sf::Event> & readInput()
	{
		return events;
	}
}