#pragma once

namespace keys
{
	void addTime(const double duration);
	bool isKeyDown(const int key, bool damp = false);
	bool keyOrLeftClick(int key, bool damp = false);
}

