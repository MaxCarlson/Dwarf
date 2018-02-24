#include "stdafx.h"
#include "KeyDampener.h"
#include <imgui.h>

namespace keys
{
	constexpr double KEY_DELAY = 75.0;
	double msSincePress = 999.0;

	void registerPress() { msSincePress = 0.0; }

	void addTime(const double duration)
	{
		msSincePress += duration;
	}

	bool readyForPress()
	{
		return msSincePress > KEY_DELAY;
	}

	bool isKeyDown(const int key, bool damp)
	{
		if (damp && !readyForPress()) 
			return false;

		if (ImGui::IsKeyDown(key))
		{
			registerPress();
			return true;
		}

		return false;
	}
}
