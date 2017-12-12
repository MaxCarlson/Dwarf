#pragma once

#include <string>
#include <vector>
#include "../BearLibTerminal.h"

namespace draw
{
	void setColor(std::string color);

	void resetColor();


	void determineHighlight(int h, int num);


	enum InputCode
	{
		IN_NOTHING,
		IN_ENTER,
		IN_EXIT
	};

	int handleInput(bool hangInput, int& selected, const std::size_t limit);


	template<typename T, bool hangInput>
	int listHandler(const std::vector<T>& vec, int &selected, int alignment, int initX, int initY, int spacing, bool clear, bool print = true, bool skipIn = false, std::string highlightC = "#d1ce38", std::string color = "default")
	{
		int panelWidth = terminal_state(TK_WIDTH);
		int panelHeight = terminal_state(TK_HEIGHT);
		const std::size_t size = vec.size();


		if (clear)
			terminal_clear();

		int spc = initY;
		int counter = 0;
		for (auto i : vec)
		{
			std::string pr = i;

			if (counter == selected)
				setColor(highlightC);
			else
				resetColor();

			terminal_print_ext(initX, spc, panelWidth, panelHeight, alignment, pr.c_str());

			++counter;
			spc += spacing;
		}

		if (print)
		{
			terminal_refresh();
			resetColor();
		}

		int code = IN_NOTHING;
		if (!skipIn)
			code = handleInput(hangInput, selected, size);

		return code;
	}
}

