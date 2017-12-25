#pragma once
#include "../Systems.h"

class InputHandler : public System<Requires<>>
{
public:
	void update();

private:
	void designate(const int key);
	void buildMenu(const int key);

	int mouseX;
	int mouseY;

	// Designation input info ~ not serialized
	enum designate_states
	{
		NONE_D,
		MINE_D,
		CHANNEL_D
	} designate_state = NONE_D;

	int desig_idx = 0;
};

