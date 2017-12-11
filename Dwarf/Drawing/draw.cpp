#include "draw.h"


namespace draw
{
	void setColor(std::string color)
	{
		terminal_bkcolor(color.c_str());
		terminal_color("black");
	}

	void resetColor()
	{
		terminal_color("default");
		terminal_bkcolor("black");
	}

	void determineHighlight(int h, int num)
	{
		if (num == h)
			setColor("#d1ce38");
		else
			resetColor();
	}

	int handleInput(bool hangInput, int & selected, const int limit)
	{
		int input = 0;
		if (!hangInput && terminal_peek())
			input = terminal_read();
		else
			input = terminal_read();

		if (input == TK_ENTER)
			return IN_ENTER;

		else if (input == TK_ESCAPE)
			return IN_EXIT;

		else if (input == TK_UP)
			--selected;

		else if (input == TK_DOWN)
			++selected;

		// Handle Selected Wrapping
		if (selected >= limit)
			selected = 0;
		else if (selected < 0)
			selected = limit - 1;

		return IN_NOTHING;
	}
}
